/* eslint-disable */

import React, { Component } from 'react'
import { Platform, StyleSheet } from 'react-native'
import { TabNavigator, TabBarTop } from 'react-navigation'
import LinearGradient from 'react-native-linear-gradient'
// import equal from 'fast-deep-equal'

const ChildrenContext = React.createContext()

let tabBarPlatformContainerStyles
if (Platform.OS === 'ios') {
  tabBarPlatformContainerStyles = {
    borderBottomWidth: StyleSheet.hairlineWidth,
    borderBottomColor: '#A7A7AA',
  }
} else {
  tabBarPlatformContainerStyles = {
    shadowColor: 'black',
    shadowOpacity: 0.1,
    shadowRadius: StyleSheet.hairlineWidth,
    shadowOffset: {
      height: StyleSheet.hairlineWidth,
    },
    elevation: 4,
  }
}

const TabBar = props => (
  <LinearGradient
    colors={['#cc1d23', '#652f8b']}
    start={{ x: 0, y: 0 }}
    end={{ x: 1, y: 0 }}
    style={tabBarPlatformContainerStyles}
  >
    <TabBarTop {...props} />
  </LinearGradient>
)

export default class SlideableTabView extends Component {
  constructor(props) {
    super(props)

    this.state = {
      navigatorClass: this.constructNavigator(props.children),
    }
  }

  // shouldComponentUpdate(nextProps) {
  //   return !equal(this.props, nextProps)
  // }

  getDerivedStateFromProps(nextProps) {
    // TODO: Ensure this works and handle title changes
    if (this.props.children.length === nextProps.children.length) return null

    return {
      navigatorClass: this.constructNavigator(nextProps.children),
    }
  }

  constructNavigator = (children) => {
    const childrenArr = Array.isArray(children) ? children : [children]
    const Navigator = TabNavigator({
      ...childrenArr.map((element, i) => {
        const screen = () => (
          <ChildrenContext.Consumer>
            {c => (Array.isArray(c) ? c[i] : c)}
          </ChildrenContext.Consumer>
        )

        screen.navigationOptions = () => {
          const c = Array.isArray(this.props.children) ? this.props.children : [this.props.children]

          return {
            title: c[i] && c[i].props && c[i].props.title,
          }
        }

        return { screen }
      }),
    }, {
      tabBarComponent: TabBar,
      tabBarPosition: 'top',
      swipeEnabled: true,
      animationEnabled: true,
      lazy: false,
      removeClippedSubviews: false,
      tabBarOptions: {
        activeTintColor: '#fff',
        labelStyle: {
          // fontSize: 12,
        },
        style: {
          backgroundColor: 'transparent',
        },
        indicatorStyle: {
          backgroundColor: '#fdc689',
        },
      },
    })
    return Navigator
  }

  render() {
    const Navigator = this.state.navigatorClass

    return (
      <ChildrenContext.Provider value={this.props.children}>
        <Navigator />
      </ChildrenContext.Provider>
    )
  }
}

export class Tab extends Component {
  render() {
    return this.props.children
  }
}
