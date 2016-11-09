var React = require('react')
  , JSX = require('node-jsx')
  , doctype = '<!DOCTYPE html>'
  , extension = '.jsx'
  ;

JSX.install({ extension: extension });

function renderFile(filename, options, cb) {
    try {
        var markup = doctype;
        var component = require(filename);
        delete options.settings;
        delete options._locals;
        delete options.cache;
        // Transpiled ES6 may export components as { default: Component }
        component = component.default || component;
        component = React.createFactory(component);
        markup += React.renderToString(component(options));
        markup += "<script>var PROPS=" + JSON.stringify(options) + "</script>";
        return cb(null, markup);
    } catch (e) {
        return cb(e);
    }
}

module.exports = renderFile;
