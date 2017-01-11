
DROP PROCEDURE IF EXISTS `CheckUserName`;
DELIMITER ;;
CREATE PROCEDURE `CheckUserName`(UserName char(32), ChannelID int unsigned, WorldID int unsigned)
BEGIN
	DECLARE AccountID INT UNSIGNED;
	SELECT account_id INTO AccountID FROM PlayerAccount where user_name=UserName and channel_id=ChannelID and world_id=WorldID;
	IF FOUND_ROWS() = 0 THEN
		INSERT INTO PlayerAccount(user_name,channel_id, world_id, create_time) VALUES(UserName, ChannelID, WorldID, unix_timestamp());
		SELECT LAST_INSERT_ID INTO AccountID;
		SELECT AccountID;
	ELSE
		SELECT AccountID;
	END IF;
END
DELIMITER ;

DROP PROCEDURE IF EXISTS `GetRoleInfo`;
DELIMITER ;;
CREATE PROCEDURE `GetRoleInfo`(AccountID int unsigned, ChannelID int unsigned, WorldID int unsigned)
BEGIN
	DECLARE RoleID INT UNSIGNED;
	SELECT role_id INTO RoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and world_id=WorldID;
	IF FOUND_ROWS() = 0 THEN
		SELECT 0;
	ELSE
		SELECT RoleID;
	END IF;
END
DELIMITER ;


DROP PROCEDURE IF EXISTS `CreateRole`;
DELIMITER ;;
CREATE PROCEDURE `CreateRole`(RoleID int unsigned, RoleName char(32), AccountID int unsigned, ChannelID int unsigned, WorldID int unsigned)
BEGIN
	DECLARE tmpRoleID INT UNSIGNED;
	INSERT INTO PlayerRole (role_id,role_name,account_id,channel_id,world_id) values(RoleID, RoleName, AccountID, ChannelID, WorldID);
	SELECT role_id INTO tmpRoleID from PlayerRole WHERE role_id=RoleID;
	IF FOUND_ROWS() = 0 THEN
		SELECT 0;
	ELSE
		SELECT tmpRoleID;
	END IF;
END
DELIMITER ;