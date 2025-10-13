-- Exported from QuickDBD: https://www.quickdatabasediagrams.com/
-- Link to schema: https://app.quickdatabasediagrams.com/#/d/OGmT4D
-- NOTE! If you have used non-SQL datatypes in your design, you will have to change these here.


CREATE TABLE `User` (
    `user_id` int AUTO_INCREMENT NOT NULL ,
    `username` VARCHAR(255)  NOT NULL ,
    PRIMARY KEY (
        `user_id`
    ),
    CONSTRAINT `uc_User_username` UNIQUE (
        `username`
    )
);

CREATE TABLE `Collection` (
    `collection_id` int AUTO_INCREMENT NOT NULL ,
    `user_id` int  NOT NULL ,
    `collection_name` VARCHAR(255)  NOT NULL ,
    PRIMARY KEY (
        `collection_id`
    )
);

CREATE TABLE `Card` (
    `card_id` VARCHAR(255)  NOT NULL ,
    `card_faces_id` VARCHAR(255)  NULL ,
    `tcgplayer_id` int  NULL ,
    `scryfall_uri` VARCHAR(255)  NOT NULL ,
    `card_name` VARCHAR(255)  NOT NULL ,
    `face_type_id` int  NOT NULL ,
    `card_face` int  NOT NULL ,
    PRIMARY KEY (
        `card_id`
    )
);

-- Information specific to a card face (This is maintained for normal cards)
CREATE TABLE `CardFace` (
    `face_id` int AUTO_INCREMENT NOT NULL ,
    `face_name` VARCHAR(255)  NOT NULL ,
    `mana_cost` VARCHAR(255)  NULL ,
    `oracle_text` VARCHAR(255)  NULL ,
    `power` int  NULL ,
    `toughness` int  NULL ,
    PRIMARY KEY (
        `face_id`
    )
);

-- Table explaining the types of allowed faces
CREATE TABLE `CardLayout` (
    `layout_id` int AUTO_INCREMENT NOT NULL ,
    `layout_type` VARCHAR(255)  NOT NULL ,
    PRIMARY KEY (
        `layout_id`
    )
);

-- Fact Table
CREATE TABLE `FaceItem` (
    `face_id` int  NOT NULL ,
    `card_id` VARCHAR(255)  NOT NULL 
);

-- Fact table
CREATE TABLE `CollectionItem` (
    `collection_id` int  NOT NULL ,
    `card_id` VARCHAR(255)  NOT NULL 
);

ALTER TABLE `Collection` ADD CONSTRAINT `fk_Collection_user_id` FOREIGN KEY(`user_id`)
REFERENCES `User` (`user_id`);

ALTER TABLE `Card` ADD CONSTRAINT `fk_Card_card_faces_id` FOREIGN KEY(`card_faces_id`)
REFERENCES `Card` (`card_id`);

ALTER TABLE `Card` ADD CONSTRAINT `fk_Card_face_type_id` FOREIGN KEY(`face_type_id`)
REFERENCES `CardLayout` (`layout_id`);

ALTER TABLE `FaceItem` ADD CONSTRAINT `fk_FaceItem_face_id` FOREIGN KEY(`face_id`)
REFERENCES `CardFace` (`face_id`);

ALTER TABLE `FaceItem` ADD CONSTRAINT `fk_FaceItem_card_id` FOREIGN KEY(`card_id`)
REFERENCES `Card` (`card_id`);

ALTER TABLE `CollectionItem` ADD CONSTRAINT `fk_CollectionItem_collection_id` FOREIGN KEY(`collection_id`)
REFERENCES `Collection` (`collection_id`);

ALTER TABLE `CollectionItem` ADD CONSTRAINT `fk_CollectionItem_card_id` FOREIGN KEY(`card_id`)
REFERENCES `Card` (`card_id`);

INSERT INTO CardLayout (layout_type) VALUES
('normal'),
('split'),
('flip'),
('transform'),
('modal_dfc'),
('meld'),
('leveler'),
('saga'),
('adventure'),
('planar'),
('scheme'),
('vanguard'),
('token'),
('class'),
('case'),
('mutate'),
('double_faced_token'),
('emblem'),
('augment'),
('host'),
('art_series'),
('reversible_card'),
('prototype');