"use strict";

module.exports = function(sequelize, DataTypes) {
    var PlayerLocation = sequelize.define("PlayerLocation", {
        x: {
            type: DataTypes.FLOAT,
            allowNull: false
        },
        y: {
            type: DataTypes.FLOAT,
            allowNull: false
        },
        z: {
            type: DataTypes.FLOAT,
            allowNull: false
        },
        dimension: {
            type: DataTypes.INTEGER,
            allowNull: false
        }
    }, {
        timestamps: true,
        updatedAt: false, // these records won't be updated
        
        classMethods: {
            associate: function(models) {
                PlayerLocation.belongsTo(models.Player, {
                    allowNull: false,
                    foreignKey: 'player'
                });
            }
        }
  });
  return PlayerLocation;
};
