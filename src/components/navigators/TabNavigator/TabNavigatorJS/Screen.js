/* eslint-disable */

import Tab from './Tab'

const Screen = props => props.navigation.state.params.content

Screen.navigationOptions = ({ navigation }) => {
  if (navigation.state.params.content.type === Tab) {
    const tabProps = navigation.state.params.content.props || {}

    return {
      title: tabProps.title,
      tabBarIcon: tabProps.tabBarIcon,
      tabBarOnPress: ({
        previousScene: { index: previousIndex },
        scene: { index },
        jumpToIndex,
      }) => {
        jumpToIndex(index)
        if (typeof tabProps.tabBarOnPress !== 'function') return
        tabProps.tabBarOnPress({ previousIndex, index })
      },
    }
  }

  return {}
}

export default Screen
