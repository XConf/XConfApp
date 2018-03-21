/* eslint-disable react/no-multi-comp */
import React, { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

export class HeaderRightContentWrapper extends Component {
  static propTypes = {
    buttonColor: PropTypes.string,
    captureEvent: PropTypes.func,
    capturedEvents: PropTypes.object,
  };

  static defaultProps = {
    buttonColor: null,
    captureEvent: null,
    capturedEvents: null,
  };

  static childContextTypes = {
    buttonColor: PropTypes.string,
    pressEventCapturingFunction: PropTypes.func,
    capturedButtonPressEvent: PropTypes.array,
  };

  getChildContext() {
    return {
      buttonColor: this.props.buttonColor,
      pressEventCapturingFunction: (
        this.props.children &&
        (typeof this.props.children === 'object' || this.props.children.length === 1) &&
        this.props.captureEvent &&
        this.props.captureEvent.bind(null, 'header-right-button-press')
      ),
      capturedButtonPressEvent: this.props.capturedEvents && this.props.capturedEvents['header-right-button-press'],
    }
  }

  render() {
    return this.props.children
  }
}

export default class HeaderRight extends Component {
  static contextTypes = {
    captureEvent: PropTypes.func,
    capturedEvents: PropTypes.object,
    setHeaderRightElement: PropTypes.func,
    getHeaderOptions: PropTypes.func,
  };

  constructor(props, context) {
    super(props, context)
    this.renderIntoPlace(props, context)
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
      if (typeof this.context.setHeaderRightElement !== 'function') return
      this.context.setHeaderRightElement(null)
    }, 100)
  }

  renderIntoPlace(props, context) {
    const p = props || this.props
    const c = context || this.context
    if (typeof c.setHeaderRightElement !== 'function') return

    const headerOptions = (c.getHeaderOptions && c.getHeaderOptions()) || {}

    const content = (
      <HeaderRightContentWrapper
        captureEvent={c.captureEvent}
        capturedEvents={c.capturedEvents}
        buttonColor={headerOptions.tintColor}
      >
        {p.children}
      </HeaderRightContentWrapper>
    )

    c.setHeaderRightElement(content)
  }

  render() {
    return null
  }
}
