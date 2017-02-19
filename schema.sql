CREATE TABLE "files" (
	`id`	INTEGER DEFAULT NULL,
	`file_id`	TEXT UNIQUE,
	`file_path`	TEXT,
	`limit_type`	TEXT DEFAULT 'C',
	`dl_counter`	INTEGER DEFAULT 0,
	`limit_timestamp`	INTEGER,
	PRIMARY KEY(id)
)