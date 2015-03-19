'use strict';

var React = require('react')
  ;

var Screenshot = module.exports = React.createClass({
    render: function() {
        var image = this.props.image;
        return (
            <div class="image" style="background-image: url(/screenshots/{image.url})">
            </div>
        );
    }
});