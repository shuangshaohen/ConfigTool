TEMPLATE=subdirs

SUBDIRS+= $$PWD/DataBase/DataBase.pro \
          $$PWD/Output/Output.pro \
          $$PWD/CommGui/CommGui.pro\
          $$PWD/TopoPara/TopoPara.pro\
          $$PWD/App/ConfigTool.pro\

CONFIG += ordered
