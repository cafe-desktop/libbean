# -*- coding: utf-8 -*-

# pythonhello.py
# This file is part of libbean
#
# Copyright (C) 2009-2010 Steve Frécinaux
#
# libbean is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# libbean is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.

from gi.repository import GObject
from gi.repository import Bean
from gi.repository import BeanCtk
from gi.repository import Ctk

LABEL_STRING="Python Says Hello!"

class PythonHelloPlugin(GObject.Object, Bean.Activatable):
    __gtype_name__ = 'PythonHelloPlugin'

    object = GObject.Property(type=GObject.Object)

    def do_activate(self):
        window = self.object
        print("PythonHelloPlugin.do_activate", repr(window))
        window._pythonhello_label = Ctk.Label()
        window._pythonhello_label.set_text(LABEL_STRING)
        window._pythonhello_label.show()
        window.get_child().pack_start(window._pythonhello_label, True, True, 0)

    def do_deactivate(self):
        window = self.object
        print("PythonHelloPlugin.do_deactivate", repr(window))
        window.get_child().remove(window._pythonhello_label)
        window._pythonhello_label.destroy()

    def do_update_state(self):
        print("PythonHelloPlugin.do_update_state", repr(self.object))

class PythonHelloConfigurable(GObject.Object, BeanCtk.Configurable):
    __gtype_name__ = 'PythonHelloConfigurable'

    def do_create_configure_widget(self):
        return Ctk.Label.new("Python Hello configure widget")

# ex:set ts=4 et sw=4 ai:
