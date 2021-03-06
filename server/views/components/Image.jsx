'use strict';

var React = require('react');

module.exports = React.createClass({

    render: function() {
        var image = this.props.image;
        var imageStyle = {
            backgroundImage: 'url(' + image.url + ')'
        };

        return (
            <div className="image" style={imageStyle} onClick={this.props.onImageClick.bind(null, image)} />
        );
    }
});