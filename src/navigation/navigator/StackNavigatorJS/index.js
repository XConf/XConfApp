/* eslint-disable react/no-multi-comp */
import React, { Component } from 'react'
import PropTypes from 'prop-types'
import ReactTestRenderer from 'react-test-renderer'
import equal from 'fast-deep-equal'

import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

const getHeaderStyleNavigationOptions = (style) => {
  switch (style) {
    case 'light':
      return {
        headerStyle: {
          backgroundColor: '#fff',
        },
        headerTintColor: '#f4511e',
      }
    default:
      return {
        headerStyle: {
          backgroundColor: '#f4511e',
        },
        headerTintColor: '#fff',
      }
  }
}

const getHeaderOptions = (style) => {
  const navigationOptions = getHeaderStyleNavigationOptions(style)
  return {
    tintColor: navigationOptions.headerTintColor,
  }
}

const screenChildContextPropTypes = {
  setHeaderStyle: PropTypes.func,
  setTitleText: PropTypes.func,
  setHeaderRightElement: PropTypes.func,
  getHeaderOptions: PropTypes.func,
}

class Screen extends Component {
  static contextTypes = {
    router: PropTypes.func.isRequired,
  };

  static navigationOptions = ({ navigation }) => {
    const params = navigation.state.params || {}
    let { headerStyle } = params

    const { mounted } = params
    const paramsFromRenderingPreConstructedElements = {}
    if (!mounted) {
      const hackyMessageStorage = params.hackyMessageStorage || {}

      const capturedEvents = {}
      const captureEvent = (type, ...args) => {
        capturedEvents[type] = args
      }
      const setHeaderStyle = (v) => {
        headerStyle = v
      }
      const setTitleText = (title) => {
        paramsFromRenderingPreConstructedElements.title = title
      }
      const setHeaderRightElement = (headerRight) => {
        paramsFromRenderingPreConstructedElements.headerRight = headerRight
      }

      class ContextProvider extends Component {
        static childContextTypes = {
          ...screenChildContextPropTypes,
          captureEvent: PropTypes.func,
        };

        getChildContext() {
          return {
            captureEvent,
            setHeaderStyle,
            setTitleText,
            setHeaderRightElement,
            getHeaderOptions: () => getHeaderOptions(headerStyle),
          }
        }

        render() {
          return this.props.children
        }
      }

      const { preConstructedElements } = params

      if (__DEV__) console.reportErrorsAsExceptions = false
      try {
        ReactTestRenderer.create(<ContextProvider>{preConstructedElements}</ContextProvider>)
      } catch (e) {}
      // try {
      //   ReactTestRenderer.create(<ContextProvider>{preConstructedElements}</ContextProvider>)
      // } catch (e) {}
      if (__DEV__) console.reportErrorsAsExceptions = true

      hackyMessageStorage.capturedEvents = capturedEvents
    }

    return {
      title: params.titleText,
      headerRight: params.headerRightElement,
      ...paramsFromRenderingPreConstructedElements,
      ...getHeaderStyleNavigationOptions(headerStyle),
    }
  };

  static childContextTypes = {
    capturedEvents: PropTypes.object,
    ...screenChildContextPropTypes,
  };

  constructor(props, context) {
    super(props, context)
  }

  getChildContext() {
    const context = {
      setHeaderStyle: this.setHeaderStyle,
      setTitleText: this.setTitleText,
      setHeaderRightElement: this.setHeaderRightElement,
      getHeaderOptions: () => getHeaderOptions(this.headerStyle),
    }

    if (
      this.props.navigation &&
      this.props.navigation.state &&
      this.props.navigation.state.params &&
      this.props.navigation.state.params.hackyMessageStorage &&
      this.props.navigation.state.params.hackyMessageStorage.capturedEvents
    ) {
      context.capturedEvents = this.props.navigation.state.params.hackyMessageStorage.capturedEvents
      this.props.navigation.state.params.hackyMessageStorage.capturedEvents = undefined
    }

    return context
  }

  componentDidMount() {
    this.props.navigation.setParams({ mounted: true })
  }

  setHeaderStyle = (headerStyle) => {
    this.headerStyle = headerStyle
    this.props.navigation.setParams({ headerStyle })
  };

  setTitleText = (titleText) => {
    this.props.navigation.setParams({ titleText })
  };

  setHeaderRightElement = (headerRightElement) => {
    this.props.navigation.setParams({ headerRightElement })
  };

  render() {
    const { params } = this.props.navigation.state
    const { route } = params
    return this.context.router(route)
  }
}

const GeneralStackNavigator = StackNavigator(
  {
    Screen: {
      screen: Screen,
    },
  },
  {
    headerTransitionPreset: 'uikit',
  },
)

const splitComplexState = (complexState) => {
  const { key, isTransitioning } = complexState
  const internalState = { key, isTransitioning }

  // if (!equal(this.state.internalState, internalState)) {
  //   this.setState({ internalState })
  // }

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

  // if (!equal(this.state.routeParamsMap, routeParamsMap)) {
  //   this.setState({ routeParamsMap })
  // }

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

const constructComplexState = ({ internalState, routeParamsMap, simpleState }) => {
  // const { routeParamsMap } = this.state
  const { routes: simpleRoutes, index } = simpleState

  const routes = simpleRoutes.map((simpleRouteEntry) => {
    const { route, key } = simpleRouteEntry

    const params = routeParamsMap[key]

    return {
      routeName: 'Screen',
      key,
      params: {
        ...params,
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
      // https://github.com/react-navigation/react-navigation/blob/4def39c0f7a462e02a684001338247d9cbc2fcdd/src/routers/StackRouter.js#L102-L103
      internalState: {
        // key: 'StackRouterRoot',
        // isTransitioning: false,
      },
      routeParamsMap: {},
    }

    this.preparePreConstructElements(props)
  }

  getChildContext() {
    const { router } = this.props

    return {
      router,
    }
  }

  // getActionForRouteEntry = ({ route, key }) => {
  //   const action = GeneralStackNavigator.router.getActionForPathAndParams('Screen', { route: route })
  //   // if (key) action.key = key // TODO: allow provide key
  //   return action
  // }

  componentWillReceiveProps(nextProps) {
    this.preparePreConstructElements(nextProps)
    this.clearUnusedRouteParamsMapItem(nextProps)
  }

  preparePreConstructElements(props) {
    const { state, router } = props
    const { routes } = state

    routes.forEach((routeEntry) => {
      const rpm = this.state.routeParamsMap[routeEntry.key]
      if (!(rpm && rpm.preConstructedElements)) {
        const preConstructedElements = router(routeEntry.route)
        this.state.routeParamsMap[routeEntry.key] = {
          ...rpm,
          preConstructedElements,
          hackyMessageStorage: {},
        }
      }
    })
  }

  clearUnusedRouteParamsMapItem(props) {
    // TODO
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
    // const prevComplexState = this.simpleStateToComplexState(this.props.state)
    // const complexState = GeneralStackNavigator.router.getStateForAction(action, prevComplexState)
    // const simpleState = this.complexStateToSimpleState(complexState)
    // if (!equal(this.props.state, simpleState)) {
      // this.props.updateState(simpleState)
    // }
  }

  render() {
    const state = constructComplexState({
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

// export const getStateForAction = (action, prevState) => {
//   // const action = NavigationActions.navigate({ routeName })
//   GeneralStackNavigator.router.getStateForAction(action, prevState)
// }

// export const navigate()

// export default class Stack extends Component {
//   constructor(props, context) {
//     super(props, context)

//     this.state = {
//       index: 0,
//       routes: [{
//         route: 0,
//         key: 'init'
//       }],
//     }
//   }

//   render() {
//     return (
//       <WrappedStackNavigator
//         state={this.state}
//         updateState={(newState) => this.setState(newState)}
//       />
//     )
//   }
// }
