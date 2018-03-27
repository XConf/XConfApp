import React, { Component } from 'react'
import PropTypes from 'prop-types'

export default class Screen extends Component {
  static contextTypes = {
    router: PropTypes.func.isRequired,
  };

  render() {
    const { params } = this.props.navigation.state
    const { route } = params
    if (typeof route === 'object') return route
    const { router } = this.context
    return router(route)
  }
}
