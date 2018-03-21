/* eslint-disable react/no-unused-prop-types */
import { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

import TitleText from './TitleText'
import HeaderRight from './HeaderRight'

export {
  TitleText,
  HeaderRight,
}

export default class StackNavigatorHeader extends Component {
  static propTypes = {
    style: PropTypes.string,
  };

  static defaultProps = {
    style: 'default',
  };

  static contextTypes = {
    setHeaderStyle: PropTypes.func,
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
      if (typeof this.context.setHeaderStyle !== 'function') return
      this.context.setHeaderStyle(null)
    }, 100)
  }

  set(props, context) {
    const p = props || this.props
    const c = context || this.context
    if (typeof c.setHeaderStyle !== 'function') return

    c.setHeaderStyle(p.style)
  }

  render() {
    return this.props.children
  }
}
