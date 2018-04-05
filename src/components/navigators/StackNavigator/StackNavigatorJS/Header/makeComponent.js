import React, { Component } from 'react'
import equal from 'fast-deep-equal'

import { getHeaderOptions } from '../utils'

import HeaderContext from '../HeaderContext'
import StyleContext from './StyleContext'
import ButtonContext from '../../../../Button/ButtonContext'

const makeComponent = (componentName, elementName) => {
  class C extends Component {
    static displayName = componentName;

    constructor(props, context) {
      super(props, context)

      this.capturePressEventKey = `${elementName}-press`
    }

    shouldComponentUpdate(nextProps) {
      return !equal(this.props, nextProps)
    }

    render() {
      const { children } = this.props

      return (
        <StyleContext.Consumer>
          {({ style }) => (
            <HeaderContext.Consumer>
              {({
                setNavigationParams,
                triggerCapturedEvent,
                eventCapturingFunction,
              }) => {
                const headerOptions = getHeaderOptions(style)

                let buttonPressEventCapturingFunction
                if (eventCapturingFunction) {
                  buttonPressEventCapturingFunction = (...args) => {
                    eventCapturingFunction(this.capturePressEventKey, args)
                  }
                }

                let triggerCapturedPressEvent
                if (triggerCapturedEvent) {
                  triggerCapturedPressEvent = func => triggerCapturedEvent(this.capturePressEventKey, func)
                }

                const content = (
                  <ButtonContext.Provider
                    triggerCapturedPressEvent={triggerCapturedPressEvent}
                    pressEventCapturingFunction={buttonPressEventCapturingFunction}
                    color={headerOptions.tintColor}
                  >
                    {children}
                  </ButtonContext.Provider>
                )

                setNavigationParams({ [elementName]: content })
              }}
            </HeaderContext.Consumer>
          )}
        </StyleContext.Consumer>
      )
    }
  }

  return C
}

export default makeComponent
