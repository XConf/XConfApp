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
  }

  componentDidMount() {
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

    this.observable = observable
    this.subscription = subscription
  }

  componentWillUnmount() {
    if (this.subscription) return
    this.subscription.unsubscribe()
  }

  refetch = async () => {
    if (!this.observable) return
    this.setState({ fetching: true })
    await this.observable.refetch()
    this.setState({ fetching: false })
  }

  render() {
    return this.props.children(this.state, { refetch: this.refetch })
  }
}
