import React, { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'

import HeaderContext from '../HeaderContext'

import StyleContext from './StyleContext'

import TitleText from './TitleText'
import makeComponent from './makeComponent'

const Right = makeComponent('Right', 'headerRight')
const Left = makeComponent('Left', 'headerLeft')

export {
  TitleText,
  Right,
  Left,
}

export default class StackNavigatorHeader extends Component {
  static propTypes = {
    style: PropTypes.string,
  };

  static defaultProps = {
    style: 'default',
  };

  shouldComponentUpdate(nextProps) {
    return !equal(this.props, nextProps)
  }

  render() {
    const { style, children } = this.props
    return (
      <HeaderContext.Consumer>
        {({ setNavigationParams }) => {
          setNavigationParams({ headerStyle: style })

          return (
            <StyleContext.Provider style={style}>
              {children}
            </StyleContext.Provider>
          )
        }}
      </HeaderContext.Consumer>
    )
  }
}
