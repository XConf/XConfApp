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

  componentWillReceiveProps(nextProps) {
    this.clearUnusedRouteParamsMapItem(nextProps)
  }

  clearUnusedRouteParamsMapItem(props) {
    // TODO: Ensure this is cleared automatically
  }

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

      if (!equal(this.props.state, simpleState)) {
        this.props.updateState(simpleState)
      }

      const state = {}

      if (!equal(prevInternalState, internalState)) state.internalState = internalState
      if (!equal(prevRouteParamsMap, routeParamsMap)) state.routeParamsMap = routeParamsMap

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
