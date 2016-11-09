/*
    The mcconfig module must be able to validate all the files that constitute a minecraft server
    configuration. Key=value pairs must be validated against a particular server version. Server properties
    must be able to be stored as json, and serialized to the proper format on the server.
 */

function getDefault(in, default) {
    return in === undefined ? default : in;
}

class PropertyValue {
    constructor(options) {
        this.required = getDefault(options.required, false);
        this.default  = getDefault(options.default,  null);
        this.choices  = getDefault(options.choices,  null);
    }

    validate(item) {
        if (this.required) {
            if (item == null) {
                return false;
            }
        }

        if (this.choices) {
            if (this.choices.indexOf(item) === -1) {
                return false;
            }
        }

        return true;
    }
}

class BooleanValue extends PropertyVaue {
    validate(item) {
        if (!super.validate(item)) return false;
        return typeof item === 'boolean';
    }
}

class IntegerValue extends PropertyValue {
    constructor(options) {
        super(options);
        this.minValue = options.minValue;
        this.maxValue = options.maxValue;
    }

    validate(item) {
        if (!super.validate(item)) return false;
        if (!(item === +item && item === (item|0))) return false;
        item = parseInt(item);
        if (this.minValue !== null && item < this.minValue) return false;
        if (this.maxValue !== null && item > this.maxValue) return false;
        return true;
    }
}

class StringValue extends PropertyValue {
    validate(item) {
        return typeof item === 'string';
    }
}

class EnumValue extends PropertyValue {
    constructor(args) {
        var options = {};
        var enums = [];
        for (int i = 0; i < arguments.length; ++i) {
            if (typeof arguments[i] === 'string') {
                enums.append(arguments[i]);
            } else {
                options = arguments[i];
                break;
            }
        }
        this.enums = enums;
        super(options);
    }

    validate = function(item) {
        return this.enums.indexOf(item) !== -1;
    }
}

class IPAddressValue extends PropertyValue {
    validate(item) {
        // validate if ip address
        return false;
    }
}

class URIValue extends PropertyValue {
    validate(arg) {
        // validate that this is a uri
        return false;
    }
}

var properties_1_8 = {
    generator_settings:   StringValue(),
    op_permission_level:  IntegerValue({default: 4, minValue: 1, maxValue: 4}),
    level_name:           StringValue({default: 'world'}),
    enable_query:         BooleanValue({default: false}),
    allow_flight:         BooleanValue({default: false}),
    announce_player_achievements: BooleanValue({default: true}),
    server_port:          IntegerValue({default: 25565}),
    level_type:           EnumValue('DEFAULT', 'FLAT', 'LARGEBIOMES', 'AMPLIFIED', 'CUSTOMIZED', {default: 'DEFAULT', serialize: 'string'}),
    enable_rcon:          BooleanValue({default: false}),
    level_seed:           StringValue({default: '', quoted: false})
    force_gamemode:       BooleanValue({default: false}),
    server_ip:            IPAddressValue(),
    max_build_height:     IntegerValue({default: 256}),
    spawn_npcs:           BooleanValue({default: true}),
    white_list:           BooleanValue({default: false}),
    spawn_animals:        BooleanValue({default: true}),
    hardcore:             BooleanValue({default: false}),
    snooper_enabled:      BooleanValue({default: true}),
    online_mode:          BooleanValue({default: true}),
    resource_pack:        URIValue(),
    pvp:                  BooleanValue({default: true}),
    difficulty:           EnumValue('PEACEFUL', 'EASY', 'NORMAL', 'HARD', {default: 'EASY', serialize: 'index'}),
    enable_command_block: BooleanValue({default: false}),
    gamemode:             EnumValue('SURVIVAL', 'CREATIVE', 'ADVENTURE', 'SPECTATOR', {default: 'SURVIVAL', serialize: 'index'}),
    player_idle_timeout:  IntegerValue({default: 0}),
    max_players:          IntegerValue({default: 20}),
    spawn_monsters:       BooleanValue({default: true}),
    generate_structures:  BooleanValue({default: true}),
    view_distance:        IntegerValue({default: 10}),
    motd:                 StringValue({default: "A Minecraft Server"})
};

class Config {
    constructor(items) {
        schema = this.getSchema();
        this.values = {};
        for (field in schema) {
            validator = schema[field];
            item = items[field];
            if (validator.required && item == null) {
                throw ConfigError('field ' + field + ' is required');
            }
            
    }

    getSchema() {
        return {};
    }
}


