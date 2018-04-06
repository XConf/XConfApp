import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

import { navigatorConfig } from './config'
import Screen from './Screen'
import ScreenContext from './ScreenContext'

/**
 * Construct A general StackNavigator.
 */

const GeneralStackNavigator = StackNavigator(
  {
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
    router,
    bsState,
    internalState,
    getRouteParams,
  }, prop) {
    switch (prop) {
      case 'index':
        return bsState[0]
      case 'routes': {
        if (bsState.cachedRoutesArray) return bsState.cachedRoutesArray
        const bsRoutes = bsState[1]
        const routesArray = routesFromBsRoutes({
          router,
          bsRoutes,
          getRouteParams,
        })
        bsState.cachedRoutesArray = routesArray // eslint-disable-line no-param-reassign
        const unusedRouteParamsKeySet = new Set(Object.keys(getRouteParams()))
        bsState.cachedRoutesArray.forEach((r, i) => {
          const { key } = r
          let params = getRouteParams()[key]
          if (!params) {
            params = {}
            getRouteParams()[key] = params // eslint-disable-line no-param-reassign
          }
          params.routesArrayIndex = i // eslint-disable-line no-param-reassign
          unusedRouteParamsKeySet.delete(key)
        })
        // TODO: Remove unusedRouteParamsKeySet
        return routesArray
      }
      default:
        return internalState[prop]
    }
  },
}

const routesFromBsRoutes = ({
  router,
  bsRoutes,
  getRouteParams,
}) => {
  let routes = []

  let ptr = bsRoutes
  while (Array.isArray(ptr)) {
    routes.unshift(ptr[0])
    ptr = ptr[1] // eslint-disable-line prefer-destructuring
  }

  routes = routes.map(bsRoute => new Proxy({
    router,
    bsRoute,
    getRouteParams,
  }, bsRouteProxyHandler))

  return routes
}

const bsRouteProxyHandler = {
  get(obj, prop) {
    const {
      router,
      bsRoute,
      getRouteParams,
    } = obj

    switch (prop) {
      case '__obj__':
        return obj
      case 'rid':
        return obj.rid
      case 'routeName':
        return 'Screen'
      case 'key': {
        const key = bsRoute[1]
        return key
      }
      case 'params': {
        const key = bsRoute[1]
        let params = getRouteParams()[key]
        if (!params) {
          params = {}
          getRouteParams()[key] = params // eslint-disable-line no-param-reassign
        }
        const paramsProxy = new Proxy({
          router,
          bsRoute,
          params,
        }, bsRouteParamsProxyHandler)
        return paramsProxy
      }
      case 'setCompareCounter':
        return (c = 2) => { obj.compareCounter = c } // eslint-disable-line no-param-reassign
      case 'compareCounter':
        if (obj.compareCounter > 0) --obj.compareCounter // eslint-disable-line no-param-reassign
        return obj.compareCounter
      default:
        return undefined
    }
  },
}

const bsRouteParamsProxyHandler = {
  get({
    router,
    bsRoute,
    params,
  }, prop) {
    switch (prop) {
      case 'router':
        return router
      case 'route':
        return bsRoute[0]
      case 'screenRef':
        return bsRoute[2]
      default:
        return params[prop]
    }
  },
}

// Handles route params update so that the navigator can let the correct scene be updated.
const routeParamsUpdated = ({ bsState, routeParams, key }) => {
  // If the scene is rendered, it must have the routes cached, which will be passed into the navigator.
  const cachedRoutesArray = bsState.cachedRoutesArray // eslint-disable-line prefer-destructuring
  if (!cachedRoutesArray) return

  // Find out the index of the updated route.
  const params = routeParams[key]
  if (!params) return
  if (params.willUnmount) return
  const routesArrayIndex = params.routesArrayIndex // eslint-disable-line prefer-destructuring
  if (typeof routesArrayIndex !== 'number') return

  // Clone the proxy and set the compare counter to ensure that the new route
  // will be compared as a different object, that triggers the scene to be updated.
  const route = cachedRoutesArray[routesArrayIndex]
  if (!route) return
  const newRoute = new Proxy(route.__obj__, bsRouteProxyHandler)
  newRoute.setCompareCounter()

  // Insert the new route into cached routes
  cachedRoutesArray[routesArrayIndex] = newRoute
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
    handleEvent: PropTypes.func.isRequired,
    getRouterUtilsByUpdateStateAndHandleEvent: PropTypes.func.isRequired,
    getUtilsAppliedRouter: PropTypes.func.isRequired,
  };

  constructor(props, context) {
    super(props, context)

    const {
      updateState,
      handleEvent,
      getRouterUtilsByUpdateStateAndHandleEvent,
      getUtilsAppliedRouter,
    } = props
    this.routerUtils = getRouterUtilsByUpdateStateAndHandleEvent(updateState, handleEvent)
    this.router = getUtilsAppliedRouter(this.routerUtils)

    this.state = {
      internalState: dummyState,
      routeParams: {},
    }
  }

  getRouteParams = () => this.state.routeParams;

  getState = () => {
    const { router, getRouteParams } = this
    const { state: bsState } = this.props
    const { internalState } = this.state

    return new Proxy({
      router,
      bsState,
      internalState,
      getRouteParams,
    }, bsStateProxyHandler)
  };

  handleDispatch = (action) => {
    switch (action.type) {
      case 'Navigation/SET_PARAMS': {
        const { key, params } = action
        return this.setState(({ routeParams }) => {
          const { state: bsState } = this.props
          routeParamsUpdated({ bsState, routeParams, key })

          const newParams = {
            ...routeParams[key],
            ...params,
            cachedParamsProxy: null,
          }

          return {
            routeParams: {
              ...routeParams,
              [key]: newParams,
            },
          }
        })
      }
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
    const { router } = this
    const state = this.getState()

    return (
      <ScreenContext.Provider router={router}>
        <GeneralStackNavigator
          navigation={addNavigationHelpers({
            dispatch: this.handleDispatch,
            state,
            addListener: () => ({ remove: () => {} }),
          })}
        />
      </ScreenContext.Provider>
    )
  }
}
