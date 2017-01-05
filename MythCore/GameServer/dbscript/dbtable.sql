DROP TABLE IF EXISTS `PlayerAccount`;
CREATE TABLE `PlayerAccount` (
	`account_id` int(11) UNSIGNED NOT NULL auto_increment,
	`channel_id` smallint(11) UNSIGNED NOT NULL,
	`world_id` smallint(11) UNSIGNED NOT NULL,
	`user_name` char(32) NOT NULL,
	`create_time` int(11) NOT NULL,
	PRIMARY KEY(`account_id`, `channel_id`, `world_id`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;
