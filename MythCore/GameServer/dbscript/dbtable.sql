DROP TABLE IF EXISTS `PlayerRole`;
CREATE TABLE `PlayerRole` (
	`role_id`				int(11) UNSIGNED			NOT NULL,
	`role_name`				char(32)					NOT NULL,
	`account_id`			int(11) UNSIGNED			NOT NULL,
	`account_name`			char(32)					NOT NULL,
	`channel_id`			smallint(11) UNSIGNED		NOT NULL,
	`server_id`				smallint(11) UNSIGNED		NOT NULL,
	`create_time`			timestamp					NOT NULL,
	`sex`					tinyint						NOT NULL,
	`metier`				tinyint						NOT NULL,
	`level`					smallint(11) UNSIGNED		NOT NULL DEFAULT  0,
	`exp`					bigint UNSIGNED				NOT NULL DEFAULT  0,
	`vip_level`				tinyint(11)					NOT NULL DEFAULT  0,
	`vip_exp`				int(11) UNSIGNED			NOT NULL DEFAULT  0,
	`money`					int(11) UNSIGNED			NOT NULL DEFAULT  0,
	`diamond`				int(11) UNSIGNED			NOT NULL DEFAULT  0,
	`last_off_time`			int(11) UNSIGNED			NOT NULL DEFAULT  0,

	PRIMARY KEY(`role_id`),
	KEY `IDX_ROLE_BY_ACCOUT_ID` (`account_id`),
	UNIQUE KEY `IDX_ROLE_NAME` (`role_name`)
)ENGINE=INNODB;

DROP TABLE IF EXISTS `PlayerRoleCool`;
CREATE TABLE `PlayerRoleCool` (
	`role_id`				int(11) UNSIGNED			NOT NULL,
	`role_name`				char(32)					NOT NULL,
	`account_id`			int(11) UNSIGNED			NOT NULL,
	`account_name`			char(32)					NOT NULL,
	`channel_id`			smallint(11) UNSIGNED		NOT NULL,
	`server_id`				smallint(11) UNSIGNED		NOT NULL,
	`create_time`			timestamp					NOT NULL,
	`sex`					tinyint						NOT NULL,
	`metier`				tinyint						NOT NULL,
	`level`					smallint(11) UNSIGNED		NOT NULL DEFAULT  0,
	`exp`					bigint UNSIGNED				NOT NULL DEFAULT  0,
	`vip_level`				tinyint(11)					NOT NULL DEFAULT  0,
	`vip_exp`				int(11) UNSIGNED			NOT NULL DEFAULT  0,
	`money`					int(11) UNSIGNED			NOT NULL DEFAULT  0,
	`diamond`				int(11) UNSIGNED			NOT NULL DEFAULT  0,
	`last_off_time`			int(11) UNSIGNED			NOT NULL DEFAULT  0,

	PRIMARY KEY(`role_id`),
	KEY `IDX_ROLE_BY_ACCOUT_ID` (`account_id`),
	UNIQUE KEY `IDX_ROLE_NAME` (`role_name`)
)ENGINE=INNODB;



DROP TABLE IF EXISTS `PlayerBaseProperty`;
CREATE TABLE `PlayerBaseProperty` (
	`role_id`				int(11) UNSIGNED			NOT NULL,
	`bag`					blob						DEFAULT NULL,
	`equip`					blob						DEFAULT NULL,
	`task`					blob						DEFAULT NULL,
	PRIMARY KEY(`role_id`)
)ENGINE=INNODB;

DROP TABLE IF EXISTS `PlayerBasePropertyCool`;
CREATE TABLE `PlayerBasePropertyCool` (
	`role_id`				int(11) UNSIGNED			NOT NULL,
	`bag`					blob						DEFAULT NULL,
	`equip`					blob						DEFAULT NULL,
	`task`					blob						DEFAULT NULL,
	PRIMARY KEY(`role_id`)
)ENGINE=INNODB;


DROP TABLE IF EXISTS `AllocateRoleId`;
CREATE TABLE `AllocateRoleId` (
  `server_id`				smallint(11) UNSIGNED		NOT NULL,
  `max_role_id`				int(11) UNSIGNED			NOT NULL,
  PRIMARY KEY  (`server_id`)
) ENGINE=INNODB;
