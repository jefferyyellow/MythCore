
DROP PROCEDURE IF EXISTS `CheckUserName`;
DELIMITER ;;
CREATE PROCEDURE `CheckUserName`(AccountName char(32), AccountID int unsigned, ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE RoleID INT UNSIGNED;
	DECLARE	TmpAccountID INT UNSIGNED; 
	DECLARE	TmpAccountIDCool INT UNSIGNED; 
	SELECT role_id INTO RoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID and account_name=AccountName;
	IF FOUND_ROWS() = 0 THEN
		SELECT role_id INTO RoleID from PlayerRoleCool where account_id=AccountID and channel_id=ChannelID and server_id=ServerID and account_name=AccountName;
		IF FOUND_ROWS() = 0 THEN
			SELECT AccountName, AccountID, 0;
		ELSE
			SELECT AccountName, AccountID, RoleID;

			START TRANSACTION;
			CALL ConverPlayRole(RoleID);
			COMMIT;

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
			INSERT INTO PlayerRole (role_id,role_name, sex, metier, account_id,channel_id,server_id) values(RoleID, RoleName, Sex, Metier, AccountID, ChannelID, ServerID);
			INSERT INTO PlayerBaseProperty (role_id) values(RoleID);
			SELECT role_id INTO tmpRoleID from PlayerRole WHERE role_id=RoleID;
			IF FOUND_ROWS() = 0 THEN
				SELECT -1;
			ELSE
				SELECT tmpRoleID;
			END IF;
		ELSE
			SELECT -2;
		END IF;
	ELSE
			SELECT -3;
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