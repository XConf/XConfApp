/* eslint-disable react/no-multi-comp */
import React, { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

export class ContentWrapper extends Component {
  static propTypes = {
    buttonColor: PropTypes.string,
    buttonPressEventCapturingFunction: PropTypes.func,
    capturedButtonPressEvent: PropTypes.array,
  };

  static defaultProps = {
    buttonColor: null,
    buttonPressEventCapturingFunction: null,
    capturedButtonPressEvent: null,
  };

  static childContextTypes = {
    buttonColor: PropTypes.string,
    buttonPressEventCapturingFunction: PropTypes.func,
    capturedButtonPressEvent: PropTypes.array,
  };

  getChildContext() {
    const buttonPressEventCapturingFunction =
      this.props.children &&
      (typeof this.props.children === 'object' || this.props.children.length === 1) &&
      this.props.buttonPressEventCapturingFunction

    return {
      buttonColor: this.props.buttonColor,
      buttonPressEventCapturingFunction,
      capturedButtonPressEvent: this.props.capturedButtonPressEvent,
    }
  }

  render() {
    return this.props.children
  }
}

const makeComponent = (componentName, contextSetElementName) => {
  class C extends Component {
    static displayName = componentName;
    static contextTypes = {
      captureEvent: PropTypes.func,
      getCapturedEvent: PropTypes.func,
      [contextSetElementName]: PropTypes.func,
      headerOptions: PropTypes.object,
    };

    constructor(props, context) {
      super(props, context)
      this.renderIntoPlace(props, context)
    }

    componentDidMount() {
      setTimeout(() => this.renderIntoPlace(), 500)
      setTimeout(() => this.renderIntoPlace(), 1000)
    }

    shouldComponentUpdate(nextProps, nextContext) {
      if (this.props === nextProps && this.context === nextContext) return false
      return !equal(this.props, nextProps) || !equal(this.context, nextContext)
    }

    componentDidUpdate() {
      this.renderIntoPlace()
    }

    componentWillUnmount() {
      setTimeout(() => {
        if (typeof this.context[contextSetElementName] !== 'function') return
        this.context[contextSetElementName](null)
      }, 100)
    }

    renderIntoPlace(props, context) {
      const p = props || this.props
      const c = context || this.context
      if (typeof c[contextSetElementName] !== 'function') return

      const headerOptions = c.headerOptions || {}

      const captureEventKey = `component-set-by-${contextSetElementName}`

      let buttonPressEventCapturingFunction
      if (c.captureEvent && typeof c.captureEvent === 'function') {
        buttonPressEventCapturingFunction = c.captureEvent.bind(null, captureEventKey)
      }

      let capturedButtonPressEvent
      if (!c.captureEvent) {
        capturedButtonPressEvent = c.getCapturedEvent(captureEventKey)
      }

      const content = (
        <ContentWrapper
          buttonPressEventCapturingFunction={buttonPressEventCapturingFunction}
          capturedButtonPressEvent={capturedButtonPressEvent}
          buttonColor={headerOptions.tintColor}
        >
          {p.children}
        </ContentWrapper>
      )

      c[contextSetElementName](content)
    }

    render() {
      return null
    }
  }

  return C
}

export default makeComponent
