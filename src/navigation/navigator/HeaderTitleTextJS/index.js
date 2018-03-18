/* eslint-disable react/no-multi-comp */
import { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

export default class HeaderTitleText extends Component {
  static contextTypes = {
    setTitleText: PropTypes.func,
    headerOptions: PropTypes.shape({
      tintColor: PropTypes.string,
    }),
  };

  constructor(props, context) {
    super(props, context)
    this.set(props, context)
  }

  shouldComponentUpdate(nextProps) {
    return !equal(this.props, nextProps)
  }

  componentDidUpdate() {
    this.set()
  }

  componentWillUnmount() {
    setTimeout(() => {
      if (typeof this.context.setTitleText !== 'function') return
      this.context.setTitleText(null)
    }, 100)
  }

  set(props, context) {
    const p = props || this.props
    const c = context || this.context
    if (typeof c.setTitleText !== 'function') return

    c.setTitleText(p.value)
  }

  render() {
    return null
  }
}
