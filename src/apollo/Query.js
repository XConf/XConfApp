import { Component } from 'react'
import PropTypes from 'prop-types'
import gql from 'graphql-tag'

export default class Query extends Component {
  static propTypes = {
    apolloClient: PropTypes.shape({
      watchQuery: PropTypes.func.isRequired,
    }).isRequired,
    query: PropTypes.shape({
      query: PropTypes.string.isRequired,
      variables: PropTypes.object.isRequired,
    }).isRequired,
    children: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props)

    this.state = {
      data: null,
      fetching: true,
      networkStatus: null,
      stale: null,
    }
    this.orgSetState = this.setState
    this.setState = (state) => {
      if (typeof state === 'function') {
        const newState = state(this.state)
        this.state = {
          ...this.state,
          ...newState,
        }
      } else {
        this.state = {
          ...this.state,
          ...state,
        }
      }
    }

    const { query, variables } = this.props.query
    const observable = this.props.apolloClient.watchQuery({
      query: gql(query),
      variables,
    })

    const subscription = observable.subscribe((next) => {
      this.setState((state) => {
        if (state.fetching && !state.data) {
          return { state, ...next, fetching: false }
        }

        return { state, ...next }
      })
    })

    this.state = {
      ...this.state,
      observable,
      subscription,
    }
  }

  componentDidMount() {
    this.setState = this.orgSetState
  }

  componentWillUnmount() {
    this.state.subscription.unsubscribe()
  }

  refetch = async () => {
    this.setState({ fetching: true })
    await this.state.observable.refetch()
    this.setState({ fetching: false })
  }

  render() {
    return this.props.children(this.state, { refetch: this.refetch })
  }
}
