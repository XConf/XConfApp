import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  StackNavigator,
  addNavigationHelpers,
} from 'react-navigation'

import { navigatorConfig } from './config'
import Screen from './Screen'

export const GeneralStackNavigator = StackNavigator(
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
    dispatch: PropTypes.func.isRequired,
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

  render() {
    console.log('SNW-R')

    return (
      <GeneralStackNavigator
        navigation={addNavigationHelpers({
          dispatch: this.props.dispatch,
          state: this.props.state,
          addListener: () => ({ remove: () => {} }),
        })}
      />
    )
  }
}
