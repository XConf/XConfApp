import React, { Component } from 'react'
import ReactTestRenderer from 'react-test-renderer'

import ScreenContext from './ScreenContext'
import HeaderContext from './HeaderContext'

import {
  CapturedEventsStorage,
  getHeaderStyleNavigationOptions,
} from './utils'

const capturedEventsStorage = new CapturedEventsStorage()

const getNavigationOptionsFromParams = (params = {}) => ({
  title: params.title,
  headerRight: params.headerRight,
  headerLeft: params.headerLeft,
  ...getHeaderStyleNavigationOptions(params.headerStyle),
})

export default class Screen extends Component {
  static navigationOptions = ({ navigation }) => {
    const params = navigation.state.params || {}

    const { mounted } = params
    if (!mounted) {
      const { key } = navigation.state
      const { router, route } = params
      const elements = router(route)

      return collectHeaderParamsFromElements(elements, key)
    }

    return getNavigationOptionsFromParams(params)
  };

  componentDidMount() {
    this.props.navigation.setParams({ mounted: true })
  }

  setNavigationParams = (params) => {
    this.props.navigation.setParams(params)
  };

  triggerCapturedEvent = (type, func) => {
    const { key } = this.props.navigation.state
    capturedEventsStorage.trigger(key, type, func)
  };

  render() {
    const { params } = this.props.navigation.state
    const { route } = params

    return (
      <ScreenContext.Consumer>
        {({ router }) => (
          <HeaderContext.Provider
            setNavigationParams={this.setNavigationParams}
            triggerCapturedEvent={this.triggerCapturedEvent}
          >
            {{
              ...router(route),
              ref: ref => params.screenRef[0] = [ref],
            }}
          </HeaderContext.Provider>
        )}
      </ScreenContext.Consumer>
    )
  }
}

/* eslint-disable react/no-multi-comp */
const collectHeaderParamsFromElements = (elements, key) => {
  const params = {}

  const setNavigationParams = (p) => {
    Object.assign(params, p)
  }

  const eventCapturingFunction = (type, ...args) => {
    capturedEventsStorage.set(key, type, args)
  }

  const elementsWithContext = (
    <React.Fragment>
      <HeaderContext.Provider
        setNavigationParams={setNavigationParams}
        eventCapturingFunction={eventCapturingFunction}
      >
        {elements}
      </HeaderContext.Provider>
    </React.Fragment>
  )

  if (__DEV__) console.reportErrorsAsExceptions = false
  try {
    ReactTestRenderer.create(elementsWithContext)
  } catch (e) {}
  if (__DEV__) console.reportErrorsAsExceptions = true

  return getNavigationOptionsFromParams(params)
}
