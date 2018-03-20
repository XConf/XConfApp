/* eslint-disable react/no-multi-comp */
import React, { Component } from 'react'
import PropTypes from 'prop-types'
import ReactTestRenderer from 'react-test-renderer'
import equal from 'fast-deep-equal'

import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

export const defaultNavigationOptions = {
  headerStyle: {
    backgroundColor: '#f4511e',
  },
  headerTintColor: '#fff',
}

const getHeaderOptions = () => {
  return {
    tintColor: defaultNavigationOptions.headerTintColor,
  }
}

const screenChildContextPropTypes = {
  setTitleText: PropTypes.func,
  setHeaderRightElement: PropTypes.func,
  headerOptions: PropTypes.shape({
    tintColor: PropTypes.string,
  }),
}

class Screen extends Component {
  static contextTypes = {
    router: PropTypes.func.isRequired,
  };

  static navigationOptions = ({ navigation, navigationOptions }) => {
    const params = navigation.state.params || {}

    const { mounted } = params
    const paramsFromRenderingPreConstructedElements = {}
    if (!mounted) {
      const setTitleText = (title) => {
        paramsFromRenderingPreConstructedElements.title = title
      }
      const setHeaderRightElement = (headerRight) => {
        paramsFromRenderingPreConstructedElements.headerRight = headerRight
      }

      class ContextProvider extends Component {
        static childContextTypes = screenChildContextPropTypes;

        getChildContext() {
          return {
            setTitleText,
            setHeaderRightElement,
            headerOptions: getHeaderOptions(),
          }
        }

        render() {
          return this.props.children
        }
      }

      const { preConstructedElements } = params

      try {
        if (__DEV__) console.reportErrorsAsExceptions = false
        ReactTestRenderer.create(<ContextProvider>{preConstructedElements}</ContextProvider>)
        if (__DEV__) console.reportErrorsAsExceptions = true
      } catch (e) {}
    }

    return {
      title: params.titleText,
      headerRight: params.headerRightElement,
      ...paramsFromRenderingPreConstructedElements,
    }
  };

  static childContextTypes = screenChildContextPropTypes;

  getChildContext() {
    return {
      setTitleText: this.setTitleText,
      setHeaderRightElement: this.setHeaderRightElement,
      headerOptions: getHeaderOptions(),
    }
  }

  componentDidMount() {
    this.props.navigation.setParams({ mounted: true })
  }

  setHeaderRightElement = (headerRightElement) => {
    this.props.navigation.setParams({ headerRightElement })
  };

  setTitleText = (titleText) => {
    this.props.navigation.setParams({ titleText })
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
    navigationOptions: defaultNavigationOptions,
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
