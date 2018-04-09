/*open ApolloLinks;*/
open ApolloInMemoryCache;

type dataObject = {. "__typename": string, "id": string, "key": string};

/* Create an InMemoryCache */
let inMemoryCache = createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

/* Create an HTTP Link */
let httpLink =
 ApolloLinks.createHttpLink(~uri="https://p0n3w91zk0.lp.gql.zone/graphql", ());

module Client =
 ReasonApollo.CreateClient(
   {
     let apolloClient =
       ReasonApollo.createApolloClient(
         ~cache=inMemoryCache /* restore method can be piped e.g. inMemoryCache |> restore(window.__APOLLO__) */,
         ~link=httpLink,
         ()
       );
   }
 );
