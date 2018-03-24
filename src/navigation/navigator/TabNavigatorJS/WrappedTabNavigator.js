import React, { Component } from 'react'
import PropTypes from 'prop-types'
import {
  TabNavigator,
  addNavigationHelpers,
} from 'react-navigation'

import Screen from './Screen'

export default class WrappedTabNavigator extends Component {
  static propTypes = {
    state: PropTypes.shape({
      index: PropTypes.number.isRequired,
    }).isRequired,
    updateState: PropTypes.func.isRequired,
  };

  getNavigator() {
    if (this.Navigator) {
      if (__DEV__) {
        if (this.getChildrenAsArray().length !== this.Navigator.tabsCount) {
          throw new Error('Dynamically changing the number of tabs of a TabNavigator is not allowed!')
        }
      }

      return this.Navigator
    }

    const routeConfigs = {
      ...this.getChildrenAsArray().map((_, i) => ({ screen: Screen })),
    }

    const Navigator = TabNavigator(routeConfigs)
    Navigator.tabsCount = Object.keys(routeConfigs).length

    this.Navigator = Navigator

    return Navigator
  }

  getChildrenAsArray() {
    let { children } = this.props

    if (!Array.isArray(children)) {
      children = [children]
    }

    return children
  }

  handleDispatch = (action) => {
    const newState = this.getNavigator().router.getStateForAction(action)

    if (newState.index !== this.props.state.index) {
      const { index } = newState
      this.props.updateState({ index })
    }
  };

  render() {
    const Navigator = this.getNavigator()

    const routes = this.getChildrenAsArray().map((child, i) => ({
      key: i.toString(),
      routeName: i.toString(),
      params: { content: child },
    }))

    return (
      <Navigator
        navigation={addNavigationHelpers({
          dispatch: this.handleDispatch,
          state: {
            ...this.props.state,
            routes,
          },
          addListener: () => ({ remove: () => {} }),
        })}
      />
    )
  }
}
