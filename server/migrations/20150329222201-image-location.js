"use strict";

module.exports = {
  up: function(migration, DataTypes, done) {
    // add altering commands here, calling 'done' when finished
    migration.addColumn('Image', 'x', {
        type: DataTypes.FLOAT,
        allowNull: false
    });

    migration.addColumn('Image', 'y', {
        type: DataTypes.FLOAT,
        allowNull: false
    });

    migration.addColumn('Image', 'z', {
        type: DataTypes.FLOAT,
        allowNull: false
    });

    migration.addColumn('Image', 'dimension', {
        type: DataTypes.INTEGER,
        allowNull: false
    });

    done();
  },

  down: function(migration, DataTypes, done) {
    // add reverting commands here, calling 'done' when finished
    migration.removeColumn('Image', 'x');
    migration.removeColumn('Image', 'y');
    migration.removeColumn('Image', 'z');
    migration.removeColumn('Image', 'dimension');
    done();
  }
};
