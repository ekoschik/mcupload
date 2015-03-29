'use strict';

var React = require('react');

module.exports = React.createClass({

    getDefaultProps: function() {
        return {
            scripts: []
        };
    },

    render: function() {

        var title = 'MCUpload';
        if (this.props.title) 
            title += ': ' + this.props.title;

        var scripts = this.props.scripts.map(function(script) {
            return (
                <script src={'"' + script + '"'}></script>
            );
        });

        return (
            <html>
                <head>
                    <title>{title}</title>
                    <meta name="viewport" content="width=device-width" />
                    <link rel="stylesheet" href="/css/style.css" />
                </head>
                <body>
                    {this.props.children}
                </body>
                <script src="//code.jquery.com/jquery-2.1.3.min.js"></script>
                <script src="//cdnjs.cloudflare.com/ajax/libs/socket.io/1.3.5/socket.io.min.js"></script>
                {scripts}
            </html>
        );
    }
});
