import React, { Component } from 'react'
import PropTypes from 'prop-types'

import ReactTestRenderer from 'react-test-renderer'

import {
  CapturedEventsStorage,
  getHeaderStyleNavigationOptions,
} from './utils'

const screenChildContextPropTypes = {
  setHeaderStyle: PropTypes.func,
  setTitleText: PropTypes.func,
  setHeaderRightElement: PropTypes.func,
  setHeaderLeftElement: PropTypes.func,
}

const capturedEventsStorage = new CapturedEventsStorage()

export default class Screen extends Component {
  static contextTypes = {
    router: PropTypes.func.isRequired,
  };

  static childContextTypes = {
    ...screenChildContextPropTypes,
    getCapturedEvent: PropTypes.func,
  };

  static navigationOptions = ({ navigation }) => {
    const params = navigation.state.params || {}
    const { key } = navigation.state
    const { headerStyle } = params

    const { mounted } = params
    if (!mounted) {
      const { router, route } = params
      const elements = router(route)

      // const captureEvent = (type, ...args) => {
      //   capturedEventsStorage.set(key, type, args)
      // }
      const captureEvent = (type) => {
        capturedEventsStorage.set(key, type, [{}])
      }

      return collectHeaderParamsFromElements(elements, captureEvent)
    }

    return {
      title: params.titleText,
      headerRight: params.headerRightElement,
      headerLeft: params.headerLeftElement,
      ...getHeaderStyleNavigationOptions(headerStyle),
    }
  };

  getChildContext() {
    return {
      setHeaderStyle: this.setHeaderStyle,
      setTitleText: this.setTitleText,
      setHeaderRightElement: this.setHeaderRightElement,
      setHeaderLeftElement: this.setHeaderLeftElement,
      getCapturedEvent: capturedEventsStorage.get.bind(null, this.props.navigation.state.key),
    }
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

  setHeaderLeftElement = (headerLeftElement) => {
    this.props.navigation.setParams({ headerLeftElement })
  };

  render() {
    const { params } = this.props.navigation.state
    const { route } = params
    const content = this.context.router(route)
    return {
      ...content,
      ref: ref => this.props.navigation.state.params.screenRef[0] = [ref],
    }
  }
}

/* eslint-disable react/no-multi-comp */
const collectHeaderParamsFromElements = (elements, captureEvent) => {
  let headerStyle
  const params = {}

  const setHeaderStyle = (hs) => {
    headerStyle = hs
  }
  const setTitleText = (title) => {
    params.title = title
  }
  const setHeaderRightElement = (headerRight) => {
    params.headerRight = headerRight
  }
  const setHeaderLeftElement = (headerLeft) => {
    params.headerLeft = headerLeft
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
        setHeaderLeftElement,
      }
    }

    render() {
      return this.props.children
    }
  }

  if (__DEV__) console.reportErrorsAsExceptions = false
  try {
    ReactTestRenderer.create(<ContextProvider>{elements}</ContextProvider>)
  } catch (e) {}
  if (__DEV__) console.reportErrorsAsExceptions = true

  return {
    ...params,
    ...getHeaderStyleNavigationOptions(headerStyle),
  }
}
