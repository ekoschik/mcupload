"use strict";

module.exports = function(sequelize, DataTypes) {
  var Image = sequelize.define("Image", {
    name: {
        type: DataTypes.TEXT,
        allowNull: false
    },
    url: {
        type: DataTypes.TEXT,
        allowNull: false
    },
    hash: {
        type: DataTypes.STRING(32),
        allowNull: false,
        unique: true
    }
  }, {
    classMethods: {
        associate: function(models) {
            Image.belongsTo(models.Player, {
                allowNull: false,
                foreignKey: 'player'
            });
        }
    }
  });
  return Image;
};
