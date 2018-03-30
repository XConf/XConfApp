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

export default class ModalStackNavigatorWrapper extends Component {
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

  constructor(props, context) {
    super(props, context)
    this.internalState = {}
  }

  getChildContext() {
    const { router } = this.props

    return {
      router,
    }
  }

  handleDispatch = (action) => {
    const newState = GeneralStackNavigator.router.getStateForAction(action, this.props.state)
    this.props.updateState(newState)
  };

  render() {
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
