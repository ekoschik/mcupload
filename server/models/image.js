"use strict";

module.exports = function(sequelize, DataTypes) {
  var Image = sequelize.define("Image", {
    url: DataTypes.TEXT,
    user: DataTypes.STRING
  }, {
    classMethods: {
      associate: function(models) {
        // associations can be defined here
      }
    }
  });
  return Image;
};
