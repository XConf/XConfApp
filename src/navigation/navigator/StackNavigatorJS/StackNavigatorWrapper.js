import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

import { navigatorConfig } from './config'
import Screen from './Screen'

const GeneralStackNavigator = StackNavigator(
  {
    Screen: {
      screen: Screen,
    },
  },
  navigatorConfig,
)

export default class StackNavigatorWrapper extends Component {
  static propTypes = {
    router: PropTypes.func.isRequired,
    state: PropTypes.shape({
      index: PropTypes.number.isRequired,
      routes: PropTypes.arrayOf(PropTypes.shape({
        key: PropTypes.string.isRequired,
        routeName: PropTypes.string.isRequired,
        params: PropTypes.shape({
          route: PropTypes.any.isRequired,
        }),
      })).isRequired,
    }).isRequired,
    updateState: PropTypes.func.isRequired,
  };

  static childContextTypes = {
    router: PropTypes.func.isRequired,
  };

  getChildContext() {
    const { router } = this.props

    return {
      router,
    }
  }

  handleDispatch = (action) => {
    console.log(action)
    const newState = GeneralStackNavigator.router.getStateForAction(action, this.props.state)
    this.props.updateState(newState)
  };

  render() {
    console.log('SNW-R')

    return (
      <GeneralStackNavigator
        navigation={addNavigationHelpers({
          dispatch: this.handleDispatch,
          state: this.props.state,
          addListener: () => ({ remove: () => {} }),
        })}
      />
    )
  }
}
