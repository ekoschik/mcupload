'use strict';

var React = require('react')
  , cx    = require('classnames')
  ;

module.exports = React.createClass({

    getDefaultProps: function() {
        return {
            hidden: true
        };
    },

    render: function() {
        var overlayClasses = cx({
            'modal-overlay': true,
            'hidden': this.props.hidden
        });
        var contentClasses = cx({
            'modal-content': true,
            'hidden': this.props.hidden
        });

        var image = this.props.image;
        var mapUrl = this.props.mapUrl;
        var imageStyle = { backgroundImage: 'url(' + image + ')' };

        var mapDiv = null;
        var modalImageClasses = cx({
            "modal-image": true,
            "full": mapUrl == null
        });

        if (mapUrl) {
            mapDiv = (
                <div className="modal-map">
                    <iframe src={mapUrl}></iframe>
                </div>
            );
        }

        return (
            <div className="modal-container" onClick={this.props.handleClick}>
                <div className={overlayClasses}></div>
                <div className={contentClasses}>
                    <div className={modalImageClasses} style={imageStyle}></div>
                    {mapDiv}
                </div>
            </div>
        );
    }
});