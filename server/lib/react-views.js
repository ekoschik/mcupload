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
        // Transpiled ES6 may export components as { default: Component }
        component = component.default || component;
        component = React.createFactory(component);
        markup += React.renderToStaticMarkup(component(options));
        return cb(null, markup);
    } catch (e) {
        return cb(e);
    }
}

module.exports = renderFile;
