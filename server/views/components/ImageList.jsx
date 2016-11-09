'use strict';

var React = require('react/addons')
  , Image = require('./Image.jsx')
  , ReactCSSTransitionGroup = React.addons.CSSTransitionGroup
  ;

module.exports = React.createClass({

    render: function() {
        var self = this;
        var images = this.props.images.map(function(image) {
            console.log(image);
            return (
                <li key={image.hash}>
                    <Image image={image} onImageClick={self.props.onImageClick} />
                </li>
            );
        });

        return (
            <div className="image-list">
                <ul>
                <ReactCSSTransitionGroup transitionName="float-in">
                    {images}
                </ReactCSSTransitionGroup>
                </ul>
            </div>
        );

    }
});