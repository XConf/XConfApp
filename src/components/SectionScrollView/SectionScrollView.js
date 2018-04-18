import React, { Component } from 'react'
import { SectionList } from 'react-native'

export default class SectionScrollView extends Component {
  render() {
    const { children, ...props } = this.props
    const sections = (Array.isArray(children) ? children : [children])
      .map(({ props: { header, children: c } }) => ({ header, data: [c] }))
    return (
      <SectionList
        ref={r => this.sectionList = r}
        {...props}
        renderItem={({ item }) => item}
        renderSectionHeader={({ section: { header } }) => header}
        keyExtractor={item => item.key}
        // Add a blank section at the top to make scrollToTop work
        // (won't ignore the first section header)
        sections={[{ header: null, data: [] }, ...sections]}
      />
    )
  }
}

export const scrollToTop = (r) => {
  if (r.sectionList) {
    r.sectionList.scrollToLocation({
      itemIndex: 0,
      sectionIndex: 0,
      viewOffset: 0,
      viewPosition: 0,
      animated: true,
    })
  }
}
