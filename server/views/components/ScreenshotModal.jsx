'use strict';

var React = require('react/addons')
  , cx    = React.addons.classSet
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
        var imageStyle = { backgroundImage: 'url(/screenshots/' + image + ')' };

        return (
            <div className="modal-container">
                <div className={overlayClasses}></div>
                <div className={contentClasses}>
                    <div className="modal-image" style={imageStyle}></div>
                    <div className="modal-map">
                        <iframe src={mapUrl}></iframe>
                    </div>
                </div>
            </div>
        );
    }
});