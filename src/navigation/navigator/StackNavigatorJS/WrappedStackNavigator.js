import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'
import equal from 'fast-deep-equal'

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

export default class WrappedStackNavigator extends Component {
  static propTypes = {
    router: PropTypes.func.isRequired,
    state: PropTypes.shape({
      index: PropTypes.number.isRequired,
      routes: PropTypes.arrayOf(PropTypes.shape({
        route: PropTypes.any.isRequired,
        key: PropTypes.string.isRequired,
      })).isRequired,
    }).isRequired,
    updateState: PropTypes.func.isRequired,
  };

  static childContextTypes = {
    router: PropTypes.func.isRequired,
  };

  constructor(props, context) {
    super(props, context)

    this.state = {
      internalState: {},
      routeParamsMap: {},
    }
  }

  getChildContext() {
    const { router } = this.props

    return {
      router,
    }
  }

  // componentDidReceiveProps(nextProps) {
  //   this.clearUnusedRouteParamsMapItem(nextProps)
  // }

  // clearUnusedRouteParamsMapItem(props) {
  //   const { routes } = props
  //   const { routeParamsMap } = this.state
  //   const routeParamsMapKeys = Object.keys(routeParamsMap)
  //   if (routes.length === routeParamsMapKeys.length) return
  //   const routeKeys = routes.map(r => r.key)
  //   routeParamsMapKeys.forEach((key) => {
  //     if (!routeKeys.includes(key)) delete routeParamsMap[key]
  //   })
  // }

  handleDispatch = (action) => {
    this.setState((prevState) => {
      const {
        internalState: prevInternalState,
        routeParamsMap: prevRouteParamsMap,
      } = prevState

      const prevComplexState = constructComplexState({
        internalState: prevInternalState,
        routeParamsMap: prevRouteParamsMap,
        simpleState: this.props.state,
      })

      const complexState = GeneralStackNavigator.router.getStateForAction(action, prevComplexState)

      const {
        internalState,
        routeParamsMap,
        simpleState,
      } = splitComplexState(complexState)

      if (!simpleStateEqual(this.props.state, simpleState)) {
        this.props.updateState(simpleState)
      }

      const state = {}

      if (!internalStateEqual(prevInternalState, internalState)) {
        state.internalState = internalState
      }
      if (action.type === 'Navigation/SET_PARAMS' && !routeParamsMapEqual(prevRouteParamsMap, routeParamsMap)) {
        state.routeParamsMap = routeParamsMap
      }

      if (Object.keys(state).length === 0) return false

      return state
    })
  }

  render() {
    const state = constructComplexState({
      router: this.props.router,
      simpleState: this.props.state,
      internalState: this.state.internalState,
      routeParamsMap: this.state.routeParamsMap,
    })

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

const splitComplexState = (complexState) => {
  const { key, isTransitioning } = complexState
  const internalState = { key, isTransitioning }

  const { index, routes: complexRoutes } = complexState
  const routeParamsMap = {}
  const routes = complexRoutes.map((complexRouteEntry) => {
    const { params, key: k } = complexRouteEntry
    const { route } = params

    routeParamsMap[k] = params

    return {
      route,
      key: k,
    }
  })

  const simpleState = {
    index,
    routes,
  }

  return {
    internalState,
    routeParamsMap,
    simpleState,
  }
}

const constructComplexState = ({
  internalState,
  routeParamsMap,
  simpleState,
  router,
}) => {
  const { routes: simpleRoutes, index } = simpleState

  const routes = simpleRoutes.map((simpleRouteEntry) => {
    const { route, key } = simpleRouteEntry

    const params = routeParamsMap[key]

    return {
      routeName: 'Screen',
      key,
      params: {
        ...params,
        router,
        route,
      },
    }
  })

  return {
    ...internalState,
    index,
    routes,
  }
}

const internalStateEqual = (a, b) => {
  return equal(a, b)
}

const routeParamsMapEqual = (a, b) => {
  return equal(a, b)
}

const simpleStateEqual = (a, b) => {
  // Check the type of the two states
  if (typeof a !== typeof b) return false
  if (typeof a !== 'object') return a === b

  // Check the index of the two states
  if (a.index !== b.index) return false

  // Check the type of route of the two states
  if (Array.isArray(a.routes) !== Array.isArray(b.routes)) return false
  if (!Array.isArray(a.routes)) return a.routes === b.routes

  // First check the length of the two routes, then loop over them and check the equality of the keys of each entry
  if (a.routes.length !== b.routes.length) return false
  for (let i = a.routes.length - 1; i >= 0; --i) {
    if (typeof a.routes[i] !== typeof b.routes[i]) return false
    if (a.routes[i].key !== b.routes[i].key) return false
  }

  // If every check passed, then we can say that the two state are equal
  return true
}
