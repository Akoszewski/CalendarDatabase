drop table if exists records;

create table records(
    id INT primary key,
    date DATE not null,
    title TEXT(128) not null,
    desc TEXT(1024) not null
);

select * from records;
