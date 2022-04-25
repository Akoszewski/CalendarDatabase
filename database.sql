drop table if exists Records;

create table Records(
    id integer primary key autoincrement,
    date DATE not null,
    title TEXT(128) not null,
    description TEXT(1024) not null
);

INSERT INTO Records (date, title, description) VALUES ('1998-12-12', 'Przykladowy tytul zadania', 'Przyklad');

select * from Records;
