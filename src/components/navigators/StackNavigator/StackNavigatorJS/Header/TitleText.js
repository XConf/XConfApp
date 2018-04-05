import React, { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

import HeaderContext from '../HeaderContext'

export default class HeaderTitleText extends Component {
  static propTypes = {
    value: PropTypes.string.isRequired,
  };

  shouldComponentUpdate(nextProps) {
    return !equal(this.props, nextProps)
  }

  render() {
    const { value } = this.props

    return (
      <HeaderContext.Consumer>
        {({ setNavigationParams }) => {
          setNavigationParams({ title: value })
        }}
      </HeaderContext.Consumer>
    )
  }
}
