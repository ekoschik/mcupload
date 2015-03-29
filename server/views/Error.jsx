var React = require('react')
  , Layout = require('./Layout.jsx')
  ;

module.exports = React.createClass({
    
    render: function() {
        return (
            <Layout title="Error">
                <h1>{this.props.message}</h1>
                <h2>{this.props.error.status}</h2>
                <pre>{this.props.error.stack}</pre>
            </Layout>
        );
    }
});
