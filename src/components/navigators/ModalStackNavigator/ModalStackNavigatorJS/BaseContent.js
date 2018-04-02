import React from 'react'

import BaseContentContext from './BaseContentContext'

const Base = () => (
  <BaseContentContext.Consumer>
    {({ content }) => content}
  </BaseContentContext.Consumer>
)

export default Base
