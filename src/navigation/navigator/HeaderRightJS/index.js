/* eslint-disable react/no-multi-comp */
import React, { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

export class HeaderRightContentWrapper extends Component {
  static propTypes = {
    buttonColor: PropTypes.string,
  };

  static defaultProps = {
    buttonColor: null,
  }

  static childContextTypes = {
    buttonColor: PropTypes.string,
  };

  getChildContext() {
    return {
      buttonColor: this.props.buttonColor,
    }
  }

  render() {
    return this.props.children
  }
}

export default class HeaderRight extends Component {
  static contextTypes = {
    setHeaderRightElement: PropTypes.func,
    headerOptions: PropTypes.shape({
      tintColor: PropTypes.string,
    }),
  };

  constructor(props, context) {
    super(props, context)
    this.renderIntoPlace(props, context)
  }

  shouldComponentUpdate(nextProps) {
    if (this.props === nextProps) return false
    return !equal(this.props, nextProps)
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

    const headerOptions = c.headerOptions || {}

    const content = (
      <HeaderRightContentWrapper
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
