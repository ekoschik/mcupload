'use strict';

module.exports = function(sequelize, DataTypes) {
    var Player = sequelize.define('Player', {
        uuid: {
            primaryKey: true,
            type: DataTypes.UUID
        },
        name: {
            type: DataTypes.STRING,
            allowNull: false
        },
        online: {
            type: DataTypes.BOOLEAN,
            allowNull: false,
            defaultValue: false
        },
        lastLogin: DataTypes.DATE
    }, {
        timestamps: true,
        createdAd: 'firstLogin', 
        classMethods: {
            associate: function(models) {
                // add associations here
            }
        }
    });
    return Player;
};
