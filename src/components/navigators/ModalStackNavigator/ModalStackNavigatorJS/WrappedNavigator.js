import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

import { navigatorConfig } from './config'
import BaseContent from './BaseContent'
import BaseContentContext from './BaseContentContext'
import Screen from './Screen'
import ScreenContext from './ScreenContext'

/**
 * Construct A general StackNavigator.
 */

const GeneralStackNavigator = StackNavigator(
  {
    BaseContent: {
      screen: BaseContent,
    },
    Screen: {
      screen: Screen,
    },
  },
  navigatorConfig,
)

/**
 * Proxy handlers to wrap Reason navigation state for React Navigation.
 */

const bsStateProxyHandler = {
  get({
    bsState,
    internalState,
  }, prop) {
    switch (prop) {
      case 'index':
        return bsState[0] + 1
      case 'routes': {
        if (bsState.cachedRoutesArray) return bsState.cachedRoutesArray
        const bsRoutes = bsState[1]
        const routesArray = routesFromBsRoutes({ bsRoutes })
        bsState.cachedRoutesArray = routesArray // eslint-disable-line no-param-reassign
        return routesArray
      }
      default:
        return internalState[prop]
    }
  },
}

const routesFromBsRoutes = ({ bsRoutes }) => {
  let routes = []

  let ptr = bsRoutes
  while (Array.isArray(ptr)) {
    routes.unshift(ptr[0])
    ptr = ptr[1] // eslint-disable-line prefer-destructuring
  }

  if (typeof PolyfilledProxy !== 'undefined') {
    routes = routes.map(v => new PolyfilledProxy({
      routeName: true,
      key: true,
      params: true,
      ...v,
    }, bsRouteProxyHandler))
  } else {
    routes = routes.map(v => new Proxy(v, bsRouteProxyHandler))
  }

  const baseRoute = {
    routeName: 'BaseContent',
    key: 'base',
  }

  return [baseRoute].concat(routes)
}

const bsRouteProxyHandler = {
  get(bsRoute, prop) {
    switch (prop) {
      case 'routeName':
        return 'Screen'
      case 'key':
        return bsRoute[1]
      case 'params':
        return { route: bsRoute[0] }
      default:
        return undefined
    }
  },
}

/**
 * A wrapped StackNavigator for Reason.
 */

const dummyState = {
  index: 0,
  routes: [{ routeName: 'Screen' }],
}

export default class WrappedNavigator extends Component {
  static propTypes = {
    state: PropTypes.array.isRequired,
    updateState: PropTypes.func.isRequired,
    children: PropTypes.any.isRequired,
    getRouterUtilsForUpdateState: PropTypes.func.isRequired,
    getUtilsAppliedRouter: PropTypes.func.isRequired,
  };

  constructor(props, context) {
    super(props, context)

    const {
      updateState,
      getRouterUtilsForUpdateState,
      getUtilsAppliedRouter,
    } = props
    this.routerUtils = getRouterUtilsForUpdateState(updateState)
    this.router = getUtilsAppliedRouter(this.routerUtils)

    this.state = {
      internalState: dummyState,
    }
  }

  getState = () => {
    const { state: bsState } = this.props
    const { internalState } = this.state

    if (typeof PolyfilledProxy !== 'undefined') {
      return new PolyfilledProxy({
        ...Object.keys(internalState).reduce((o, k) => { o[k] = true; return o; }, {}),
        index: true,
        routes: true,
        bsState,
        internalState,
      }, bsStateProxyHandler)
    } else {
      return new Proxy({
        bsState,
        internalState,
      }, bsStateProxyHandler)
    }
  };

  handleDispatch = (action) => {
    switch (action.type) {
      case 'Navigation/BACK':
        return this.routerUtils[1/* popRoute */]()
      default:
        this.setState(({ internalState }) => ({
          internalState: GeneralStackNavigator.router.getStateForAction(action, internalState),
        }))
        return null
    }
  };

  render() {
    const state = this.getState()
    const { router } = this
    const { children: content } = this.props

    return (
      <BaseContentContext.Provider content={content}>
        <ScreenContext.Provider router={router}>
          <GeneralStackNavigator
            navigation={addNavigationHelpers({
              dispatch: this.handleDispatch,
              state,
              addListener: () => ({ remove: () => {} }),
            })}
          />
        </ScreenContext.Provider>
      </BaseContentContext.Provider>
    )
  }
}
