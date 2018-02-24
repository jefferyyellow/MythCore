DROP TABLE IF EXISTS `PlayerAccount`;
CREATE TABLE `PlayerAccount` (
	`account_id`			int(11) UNSIGNED			NOT NULL AUTO_INCREMENT,
	`channel_id`			smallint(11) UNSIGNED		NOT NULL,
	`server_id`				smallint(11) UNSIGNED		NOT NULL,
	`user_name`				char(32)					NOT NULL,
	`create_time`			timestamp					NOT NULL,
	PRIMARY KEY(`account_id`, `channel_id`, `server_id`)
)ENGINE=INNODB;

DROP TABLE IF EXISTS `PlayerRole`;
CREATE TABLE `PlayerRole` (
	`role_id`				int(11) UNSIGNED			NOT NULL,
	`role_name`				char(32)					NOT NULL,
	`channel_id`			smallint(11) UNSIGNED		NOT NULL,
	`server_id`				smallint(11) UNSIGNED		NOT NULL,
	`account_id`			int(11) UNSIGNED			NOT NULL,
	`level`					smallint(11) UNSIGNED		NOT NULL DEFAULT '0',
	`exp`					bigint UNSIGNED				NOT NULL DEFAULT '0',
	`vip_level`				tinyint(11)					NOT NULL DEFAULT '0',
	`vip_exp`				int(11) UNSIGNED			NOT NULL DEFAULT '0',
	`money`					int(11) UNSIGNED			NOT NULL DEFAULT '0',
	`diamond`				int(11) UNSIGNED			NOT NULL DEFAULT '0',
	`last_off_time`			int(11) UNSIGNED			NOT NULL DEFAULT  0,
	PRIMARY KEY(`role_id`),
	KEY `IDX_ROLE_BY_ACCOUT_ID` (`account_id`)
)ENGINE=INNODB;

DROP TABLE IF EXISTS `PlayerBaseProperty`;
CREATE TABLE `PlayerBaseProperty` (
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
