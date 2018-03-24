import Tab from './Tab'

const Screen = props => props.navigation.state.params.content

Screen.navigationOptions = ({ navigation }) => {
  if (navigation.state.params.content.type === Tab) {
    const tabProps = navigation.state.params.content.props || {}

    return {
      title: tabProps.title,
      tabBarIcon: tabProps.tabBarIcon,
    }
  }

  return {}
}

export default Screen
