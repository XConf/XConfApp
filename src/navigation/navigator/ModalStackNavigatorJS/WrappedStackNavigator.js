import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

import { navigatorConfig } from './config'
import Screen from './Screen'

const GeneralStackNavigator = StackNavigator(
  {
    Screen: {
      screen: Screen,
    },
  },
  navigatorConfig,
)

const reasonStateWithBaseChildrenAndInternalStateAndCacheProxyHandler = {
  get({ reasonState, internalState, cache, baseChildren }, prop) {
    switch (prop) {
      case 'index':
        return reasonState[0] + 1
      case 'routes':
        return cache.routes || (cache.routes = routesFromReasonRoutes({ reasonRoutes: reasonState[1], baseChildren }))
      default:
        return internalState[prop]
    }
  },
}

const routesFromReasonRoutes = ({ reasonRoutes, baseChildren }) => {
  const routesMap = { 0: [baseChildren, baseChildren] }
  let i = 1
  let ptr = reasonRoutes
  while (Array.isArray(ptr)) {
    routesMap[i] = ptr[0]
    ptr = ptr[1]
    ++i
  }

  return Object.keys(routesMap).map(k => new Proxy({ reasonRoute: routesMap[k] }, reasonRouteProxyHandler))
}

const reasonRouteProxyHandler = {
  get({ reasonRoute }, prop) {
    switch (prop) {
      case 'routeName':
        return 'Screen'
      case 'key':
        return reasonRoute[1]
      case 'params':
        return { route: reasonRoute[0] }
      default:
        return undefined
    }
  },
}

export default class WrappedStackNavigator extends Component {
  static propTypes = {
    router: PropTypes.func.isRequired,
    state: PropTypes.array.isRequired,
    handleBack: PropTypes.func.isRequired,
  };

  static childContextTypes = {
    router: PropTypes.func.isRequired,
  };

  constructor(props, context) {
    super(props, context)
    this.internalState = {}
  }

  getChildContext() {
    const { router } = this.props

    return {
      router,
    }
  }

  getState = () => {
    const { state: reasonState, children: baseChildren } = this.props
    const { internalState } = this
    const cache = {}

    return new Proxy({ reasonState, internalState, cache, baseChildren }, reasonStateWithBaseChildrenAndInternalStateAndCacheProxyHandler)
  }

  handleDispatch = (action) => {
    switch (action.type) {
      case 'Navigation/BACK':
        return this.props.handleBack()
      default:
        return null
    }
  };

  render() {
    const state = this.getState()
    // const { state: reasonState } = this.props
    // const {index} = state
    // debugger
    // const Text = require('react-native').Text
    // return <Text>{`\n${JSON.stringify(this.props)}`}</Text>
    return (
      <GeneralStackNavigator
        navigation={addNavigationHelpers({
          dispatch: this.handleDispatch,
          state,
          addListener: () => ({ remove: () => {} }),
        })}
      />
    )
  }
}
