
DROP PROCEDURE IF EXISTS `CheckUserName`;
DELIMITER ;;
CREATE PROCEDURE `CheckUserName`(AccountName char(32), AccountID int unsigned, ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE RoleID INT UNSIGNED;
	DECLARE TmpValue INT UNSIGNED;
	SELECT role_id INTO RoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID and account_name=AccountName;
	IF FOUND_ROWS() = 0 THEN
		SELECT role_id INTO RoleID from PlayerRoleCool where account_id=AccountID and channel_id=ChannelID and server_id=ServerID and account_name=AccountName;
		IF FOUND_ROWS() = 0 THEN
			SELECT AccountName, AccountID, 0;
		ELSE
			START TRANSACTION;
			SELECT ConverPlayRole(RoleID) INTO TmpValue;
			COMMIT;
			SELECT AccountName, AccountID, RoleID;
		END IF;
	ELSE
		SELECT AccountName, AccountID, RoleID;
	END IF;
END
;;
DELIMITER ;

DROP FUNCTION IF EXISTS `ConverPlayRole`;
DELIMITER ;;
CREATE FUNCTION `ConverPlayRole`(RoleID int unsigned) RETURNS INT
BEGIN
	DELETE FROM PlayerRole WHERE role_id=RoleID;
	DELETE FROM PlayerBaseProperty WHERE role_id=RoleID;
	
	INSERT INTO PlayerRole SELECT * FROM PlayerRoleCool;
	INSERT INTO PlayerBaseProperty SELECT * FROM PlayerBasePropertyCool;
	
	DELETE FROM PlayerRoleCool WHERE role_id=RoleID;
	DELETE FROM PlayerBasePropertyCool WHERE role_id=RoleID;

	RETURN 0;
END
;;
DELIMITER ;


DROP PROCEDURE IF EXISTS `GetRoleInfo`;
DELIMITER ;;
CREATE PROCEDURE `GetRoleInfo`(AccountID int unsigned, ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE RoleID INT UNSIGNED;
	SELECT role_id INTO RoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID;
	IF FOUND_ROWS() = 0 THEN
		SELECT 0;
	ELSE
		SELECT RoleID;
	END IF;
END
;;
DELIMITER ;


DROP PROCEDURE IF EXISTS `CreateRole`;
DELIMITER ;;
CREATE PROCEDURE `CreateRole`(RoleID int unsigned, RoleName char(32), Sex tinyint, Metier tinyint, AccountName char(32), AccountID int unsigned, ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE tmpRoleID INT UNSIGNED;
	SELECT role_id INTO tmpRoleID from PlayerRole where role_name=RoleName;
	IF FOUND_ROWS() = 0 THEN
		SELECT role_id INTO tmpRoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID and account_name=AccountName;
		IF FOUND_ROWS() = 0 THEN
			INSERT INTO PlayerRole (role_id,role_name, sex, metier, account_id,account_name,channel_id,server_id) values(RoleID, RoleName, Sex, Metier, AccountID, AccountName, ChannelID, ServerID);
			INSERT INTO PlayerBaseProperty (role_id) values(RoleID);
			SELECT role_id INTO tmpRoleID from PlayerRole WHERE role_id=RoleID;
			IF FOUND_ROWS() = 0 THEN
				SELECT -1, 0;
			ELSE
				SELECT 0, tmpRoleID;
			END IF;
		ELSE
			SELECT -2, 0;
		END IF;
	ELSE
			SELECT -3, 0;
	END IF;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `LoadPlayerInfo`;
DELIMITER ;;
CREATE PROCEDURE `LoadPlayerInfo`(RoleID int unsigned)
BEGIN
	select role_name, level, exp, vip_level, vip_exp, money, diamond, last_off_time from PlayerRole WHERE role_id=RoleID;
END
;;
DELIMITER ;


DROP PROCEDURE IF EXISTS `LoadPlayerBaseProperty`;
DELIMITER ;;
CREATE PROCEDURE `LoadPlayerBaseProperty`(RoleID int unsigned)
BEGIN
	select bag, equip, task from PlayerBaseProperty WHERE role_id=RoleID;
END
;;
DELIMITER ;


DROP PROCEDURE IF EXISTS `UpdatePlayerInfo`;
DELIMITER ;;
CREATE PROCEDURE `UpdatePlayerInfo`(RoleID int unsigned,
									tLevel smallint UNSIGNED, 
									tExp bigint UNSIGNED,
									tVipLevel tinyint,
									tVipExp int,
									tMoney int,
									tDiamond int,
									tLastOffTime int)
BEGIN
	update PlayerRole set
		level = tLevel,
		exp = tExp,
		vip_level = tVipLevel,
		vip_exp = tVipExp,
		money = tMoney,
		diamond = tDiamond,
		last_off_time = tLastOffTime
	WHERE role_id=RoleID;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `UpdatePlayerBaseProperty`;
DELIMITER ;;
CREATE PROCEDURE `UpdatePlayerBaseProperty`(RoleID int unsigned,
											tBag Blob,
											tEquip Blob,
											tTask Blob)
BEGIN
	update PlayerBaseProperty set
		bag = tBag,
		equip = tEquip,
		task = tTask
	WHERE role_id=RoleID;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `InsertRechargeCache`;
DELIMITER ;;
CREATE PROCEDURE `InsertRechargeCache`(OrderIDCRC int unsigned,
										 OrderID char(64),
										 GoodsID char(64),
										 RoleID int unsigned,
										 AccountID int unsigned,
										 ChannelID int unsigned, 
										 ServerID int unsigned,
										 RechargeMoney int,
										 RechargeTime int)
BEGIN
	DECLARE tmpOrderIDCRC int unsigned;
	SELECT order_id_crc INTO OrderIDCRC from RechargeCache where order_id_crc=OrderIDCRC and 'order_id'=OrderID;
	IF FOUND_ROWS() = 0 THEN
		SELECT order_id_crc INTO OrderIDCRC from RechargeLog where order_id_crc=OrderIDCRC and 'order_id'=OrderID;
		IF FOUND_ROWS() = 0 THEN
			insert into RechargeCache(order_id_crc,order_id,goods_id,role_id,account_id,channel_id,server_id,recharge_money,recharge_time) \
			values(OrderIDCRC, OrderID, GoodsID, RoleID, AccountID, ChannelID, ServerID, RechargeMoney, RechargeTime);
			select 0;
		ELSE
			select -1;
		END IF;
	ELSE
			select -2;
	END IF;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `LoadRechargeCache`;
DELIMITER ;;
CREATE PROCEDURE `LoadRechargeCache`(RoleID int unsigned)
BEGIN
	SELECT id, order_id, goods_id, recharge_money from RechargeCache where role_id=RoleID;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `RechargeSuccess`;
DELIMITER ;;
CREATE PROCEDURE `RechargeSuccess`(RechargeID int unsigned)
BEGIN
	START TRANSACTION;
	insert into RechargeLog(order_id_crc,order_id,goods_id,role_id,account_id,channel_id,server_id,recharge_money,recharge_time) \
	select order_id_crc,order_id,goods_id,role_id,account_id,channel_id,server_id,recharge_money,recharge_time from RechargeCache where id=RechargeID;
	delete from RechargeCache where id=RechargeID;
	COMMIT;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `SaveMail`;
DELIMITER ;;
CREATE PROCEDURE `SaveMail`(MailID int unsigned,
							MailStatus tinyint unsigned,
							RoleID int unsigned,
							MailType tinyint unsigned,
							ItemLog smallint unsigned,
							CreateTime int unsigned,
							MailTitle varchar(64),
							MailBody varchar(300), 
							MailItem blob)
BEGIN
	insert into Mail(mail_id, mail_status, role_id, mail_type, item_log, create_time, mail_title, mail_body, mail_item)
	values(MailID, MailStatus, RoleID, MailType, ItemLog, CreateTime, MailTitle, MailBody, MailItem);
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `SaveGlobalMail`;
DELIMITER ;;
CREATE PROCEDURE `SaveGlobalMail`(MailID int unsigned,
							MailType tinyint unsigned,
							CreateTime int unsigned,
							MailTitle varchar(64),
							MailBody varchar(300), 
							MailItem blob)
BEGIN
	insert into GlobalMail(mail_id, mail_type, create_time, mail_title, mail_body, mail_item)
	values(MailID, MailType, CreateTime, MailTitle, MailBody, MailItem);
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `SaveGameEvent`;
DELIMITER ;;
CREATE PROCEDURE `SaveGameEvent`(EventType tinyint unsigned,
							RoleID int unsigned,
							CreateTime int unsigned,
							EventParam blob)
BEGIN
	insert into GlobalMail(role_id, event_type, create_time, event_param)
	values(RoleID, EventType, CreateTime, EventParam);
END
;;
DELIMITER ;