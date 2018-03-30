import React, { Component } from 'react'
import PropTypes from 'prop-types'
import equal from 'fast-deep-equal'
import { BsProxy, BsProxyUpdate } from '../../../utils/bs-proxy'
import ModalStackNavigatorWrapper from './ModalStackNavigatorWrapper'

export default class ReasonModalStackNavigatorWrapper extends Component {
  static propTypes = {
    router: PropTypes.func.isRequired,
    state: PropTypes.any.isRequired,
    updateState: PropTypes.func.isRequired,
  };

  constructor(props, context) {
    super(props, context)

    const routeEntryProxyOptions = {
      type: 'object',
      as: 'record',
      properties: [
        {
          name: 'route',
          resolver: o => o.params && o.params.route,
          updator: (o, v) => o.params = { ...o.params, route: v }, // eslint-disable-line no-param-reassign
        },
        { name: 'key' },
      ],
    }

    const stateProxyOptions = {
      type: 'object',
      as: 'record',
      properties: [
        {
          name: 'index',
          resolver: o => o.index - 1,
          updator: (o, v) => o.index = v + 1, // eslint-disable-line no-param-reassign
        },
        {
          name: 'routes',
          updator: (o, v) => {
            o.routes = v // eslint-disable-line no-param-reassign

            if (!o.routes[0] || o.routes[0].key !== 'main') {
              o.routes = [{ key: 'main' }].concat(o.routes) // eslint-disable-line no-param-reassign
            }

            o.routes.forEach((r) => {
              if (r.routeName !== 'Screen') r.routeName = 'Screen' // eslint-disable-line no-param-reassign
            })
          },
          value: {
            type: 'array',
            as: 'list',
            options: {
              startIndex: 1,
              reverse: true,
              value: routeEntryProxyOptions,
            },
          },
        },
      ],
    }

    this.bsProxy = new BsProxy()
    this.bsProxyUpdate = new BsProxyUpdate()

    this.proxiedState = this.bsProxy.proxied.bind(this.bsProxy, stateProxyOptions)
    this.updatedState = (originalState, bsState) => this.proxiedState((
      this.bsProxyUpdate.updated(stateProxyOptions, originalState, bsState)
    ))

    this.translateState(props)
  }

  shouldComponentUpdate(nextProps) {
    return (
      this.translateState(nextProps) ||
      nextProps.state[0] !== this.props.state[0] ||
      nextProps.children !== this.props.children
    )
  }

  translateState(nextProps) {
    let [state] = nextProps.state
    let changed = false

    if (!state.__bs_proxy__) {
      this.nextProps = nextProps
      const prevRawState = this.props.state[0].__raw__
      state = this.updatedState(prevRawState, state)
      nextProps.state[0] = state // eslint-disable-line no-param-reassign

      changed = true
    }

    if (
      !equal(nextProps.children, this.props.children) ||
      !nextProps.state[0].__raw__.routes[0].params
    ) {
      const oldState = nextProps.state[0].__raw__
      const oldMainRoute = oldState.routes[0]
      const newState = {
        ...oldState,
        routes: [
          {
            ...oldMainRoute,
            params: {
              ...oldMainRoute.params,
              route: nextProps.children,
            },
          },
          ...oldState.routes.slice(1),
        ],
      }

      nextProps.state[0] = this.proxiedState(newState) // eslint-disable-line no-param-reassign
      changed = true
    }

    return changed
  }

  handleUpdateState = (newState) => {
    this.props.updateState(this.proxiedState(newState))
  }

  render() {
    return (
      <ModalStackNavigatorWrapper
        router={this.props.router}
        state={this.props.state[0].__raw__}
        updateState={this.handleUpdateState}
      />
    )
  }
}
