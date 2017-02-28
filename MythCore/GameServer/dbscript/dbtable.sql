DROP TABLE IF EXISTS `PlayerAccount`;
CREATE TABLE `PlayerAccount` (
	`account_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,
	`channel_id` smallint(11) UNSIGNED NOT NULL,
	`world_id` smallint(11) UNSIGNED NOT NULL,
	`user_name` char(32) NOT NULL,
	`create_time` int(11) NOT NULL,
	PRIMARY KEY(`account_id`, `channel_id`, `world_id`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `PlayerRole`;
CREATE TABLE `PlayerRole` (
	`role_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,
	`role_name` char(32) NOT NULL,
	`channel_id` smallint(11) UNSIGNED NOT NULL,
	`world_id` smallint(11) UNSIGNED NOT NULL,
	`account_id` int(11) UNSIGNED NOT NULL,
	`role_level` int(11) UNSIGNED NOT NULL,
	`role_exp` bigint UNSIGNED NOT NULL,
	PRIMARY KEY(`role_id`),
	KEY `IDX_ROLE_BY_ACCOUT_ID` (`account_id`)
)ENGINE=INNODB DEFAULT CHARSET=utf8;