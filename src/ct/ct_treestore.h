/*
 * ct_treestore.h
 *
 * Copyright 2009-2025
 * Giuseppe Penone <giuspen@gmail.com>
 * Evgenii Gurianov <https://github.com/txe>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include "ct_types.h"
#include <gtkmm.h>
#include <set>
#include <unordered_map>

class CtMainWin;
class CtAnchoredWidget;
class CtTreeView;

struct CtNodeData
{
    gint64         nodeId{0};
    gint64         sharedNodesMasterId{0};
    gint64         sequence{-1};
    // the following fields are ignored in a shared node, the master id's are used
    Glib::ustring  name;
    std::string    syntax;
    Glib::ustring  tags;
    bool           isReadOnly{false};
    guint32        customIconId{0};
    bool           isBold{false};
    bool           excludeMeFromSearch{false};
    bool           excludeChildrenFromSearch{false};
    std::string    foregroundRgb24;
    gint64         tsCreation{0};
    gint64         tsLastSave{0};
    Glib::RefPtr<Gtk::TextBuffer> pTextBuffer;
    std::list<CtAnchoredWidget*> anchoredWidgets;
};

struct CtTreeModelColumns : public Gtk::TreeModelColumnRecord
{
    CtTreeModelColumns() {
        add(rColPixbuf); add(colNodeName); add(rColTextBuffer); add(colNodeUniqueId); add(colSharedNodesMasterId);
        add(colSyntaxHighlighting); add(colNodeSequence); add(colNodeTags); add(colNodeIsReadOnly);
        add(colNodeIsExcludedFromSearch); add(colNodeChildrenAreExcludedFromSearch);
        add(rColPixbufAux); add(colCustomIconId); add(colWeight); add(colForeground);
        add(colTsCreation); add(colTsLastSave); add(colAnchoredWidgets);
    }
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>>    rColPixbuf;
    Gtk::TreeModelColumn<Glib::ustring>                colNodeName;
    Gtk::TreeModelColumn<Glib::RefPtr<Gtk::TextBuffer>>  rColTextBuffer;
    Gtk::TreeModelColumn<gint64>                       colNodeUniqueId;
    Gtk::TreeModelColumn<gint64>                       colSharedNodesMasterId;
    Gtk::TreeModelColumn<std::string>                  colSyntaxHighlighting;
    Gtk::TreeModelColumn<gint64>                       colNodeSequence;
    Gtk::TreeModelColumn<Glib::ustring>                colNodeTags;
    Gtk::TreeModelColumn<bool>                         colNodeIsReadOnly;
    Gtk::TreeModelColumn<bool>                         colNodeIsExcludedFromSearch;
    Gtk::TreeModelColumn<bool>                         colNodeChildrenAreExcludedFromSearch;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>>    rColPixbufAux;
    Gtk::TreeModelColumn<guint16>                      colCustomIconId;
    Gtk::TreeModelColumn<int>                          colWeight;
    Gtk::TreeModelColumn<std::string>                  colForeground;
    Gtk::TreeModelColumn<gint64>                       colTsCreation;
    Gtk::TreeModelColumn<gint64>                       colTsLastSave;
    Gtk::TreeModelColumn<std::list<CtAnchoredWidget*>> colAnchoredWidgets;
};

class CtMainWin;

class CtTreeIter : public Gtk::TreeModel::iterator
{
public:
    CtTreeIter(Gtk::TreeModel::iterator iter, const CtTreeModelColumns* _columns, CtMainWin* pCtMainWin);
    CtTreeIter() {} // invalid, casting to bool will give false

    CtTreeIter  parent() const;
    CtTreeIter  first_child() const;

    bool          get_node_is_excluded_from_search() const;
    void          set_node_is_excluded_from_search(const bool val);
    bool          get_node_children_are_excluded_from_search() const;
    void          set_node_children_are_excluded_from_search(const bool val);
    bool          get_node_is_bold() const;
    bool          get_node_read_only() const;
    void          set_node_read_only(const bool val);
    gint64        get_node_sequence() const;
    gint64        get_node_id() const;
    void          set_node_id(const gint64 new_id);
    gint64        get_node_shared_master_id() const;
    void          set_node_shared_master_id(const gint64 new_master_id);
    gint64        get_node_id_data_holder() const;
    std::vector<gint64> get_children_node_ids() const;
    guint16       get_node_custom_icon_id() const;
    Glib::RefPtr<Gdk::Pixbuf> get_node_icon() const;
    Glib::ustring get_node_name() const;
    void          set_node_name(const Glib::ustring& node_name);
    Glib::ustring get_node_tags() const;
    std::string   get_node_foreground() const;
    std::string   get_node_syntax_highlighting() const;
    bool          get_node_is_rich_text() const;
    bool          get_node_is_plain_text() const;
    bool          get_node_is_text() const;
    bool          get_node_is_code() const { return not get_node_is_text(); }
    gint64        get_node_creating_time() const;
    gint64        get_node_modification_time() const;
    void          set_node_modification_time(const gint64 modification_time);
    void          set_node_sequence(gint64 num);

    void                      set_node_text_buffer(Glib::RefPtr<Gtk::TextBuffer> new_buffer, const std::string& new_syntax_highlighting);
    Glib::RefPtr<Gtk::TextBuffer> get_node_text_buffer() const;
    bool                      get_node_buffer_already_loaded() const;

    void                         remove_all_embedded_widgets();
    std::list<CtAnchoredWidget*> get_anchored_widgets_fast(const char doSort = 'n') const;
    std::list<CtAnchoredWidget*> get_anchored_widgets(const int start_offset = -1,
                                                      const int end_offset = -1,
                                                      const bool also_links = false) const;
    CtAnchoredWidget*            get_anchored_widget(Glib::RefPtr<Gtk::TextChildAnchor> rChildAnchor) const;

    void pending_edit_db_node_prop();
    void pending_edit_db_node_buff();
    void pending_edit_db_node_hier();
    void pending_new_db_node();

    const char* get_cherrytree_filepath() const;

    static int  get_pango_weight_from_is_bold(const bool isBold);
    static bool get_is_bold_from_pango_weight(const int pangoWeight);

    static bool get_hit_exclusion_from_search() { return _hitExclusionFromSearch; }
    static void clear_hit_exclusion_from_search() { _hitExclusionFromSearch = false; }

private:
    const CtTreeModelColumns* _pColumns{nullptr};
    CtMainWin*                _pCtMainWin{nullptr};

    static bool _hitExclusionFromSearch;
};

class CtTextView;

class CtTreeStore : public sigc::trackable
{
public:
    CtTreeStore(CtMainWin* pCtMainWin);
    virtual ~CtTreeStore();

    void          tree_view_connect(Gtk::TreeView* pTreeView);
    void          text_view_apply_textbuffer(CtTreeIter& treeIter, CtTextView* pTextView);

    void          get_node_data(const Gtk::TreeModel::iterator& treeIter, CtNodeData& nodeData, const bool loadTextBuffer);
    bool          populate_summary_info(CtSummaryInfo& summaryInfo);
    unsigned      tree_clear_property_exclude_from_search();
    unsigned      populate_shared_nodes_map(CtSharedNodesMap& sharedNodesMap) const;

    void          update_node_data(const Gtk::TreeModel::iterator& treeIter, const CtNodeData& nodeData);
    void          update_node_icon(const Gtk::TreeModel::iterator& treeIter);
    void          update_nodes_icon(Gtk::TreeModel::iterator father_iter,  bool cherry_only);
    void          update_node_aux_icon(const Gtk::TreeModel::iterator& treeIter);

    Gtk::TreeModel::iterator append_node(CtNodeData* pNodeData, const Gtk::TreeModel::iterator* pParentIter=nullptr);
    Gtk::TreeModel::iterator insert_node(CtNodeData* pNodeData, const Gtk::TreeModel::iterator& afterIter);

    void addAnchoredWidgets(CtTreeIter ctTreeIter, std::list<CtAnchoredWidget*> anchoredWidgetList, Gtk::TextView* pTextView);

    void treeview_set_tree_path_n_text_cursor(CtTreeView* pTreeView,
                                              const std::string& node_path,
                                              const int cursor_pos,
                                              const int v_adj_val);
    std::string treeview_get_tree_expanded_collapsed_string(Gtk::TreeView& treeView);
    void        treeview_set_tree_expanded_collapsed_string(const std::string& expanded_collapsed_string, Gtk::TreeView& treeView, bool nodes_bookm_exp);

    gint64                         node_id_get(gint64 original_id=-1,
                                               std::unordered_map<gint64,gint64> remapping_ids=std::unordered_map<gint64,gint64>{});
    void                           add_used_tags(const Glib::ustring& tags);
    const std::set<Glib::ustring>& get_used_tags() { return _usedTags; }
    bool                           is_node_bookmarked(const gint64 node_id);
    std::string                    get_node_name_from_node_id(const gint64 node_id);
    CtTreeIter                     get_node_from_node_id(const gint64 node_id);
    CtTreeIter                     get_node_from_node_name(const Glib::ustring& node_name);

    bool                           bookmarks_add(gint64 nodeId);
    bool                           bookmarks_remove(gint64 nodeId);
    const std::list<gint64>&       bookmarks_get();
    void                           bookmarks_set(const std::list<gint64>& bookmarks);

    Glib::RefPtr<Gtk::TreeStore>    get_store() { return _rTreeStore; }
    Gtk::TreeModel::iterator                   get_iter_first();
    CtTreeIter                      get_ct_iter_first();
    Gtk::TreeModel::iterator                   get_tree_iter_last_sibling(const Gtk::TreeNodeChildren& children);
    Gtk::TreePath                   get_path(Gtk::TreeModel::iterator tree_iter);
    CtTreeIter                      get_iter(Gtk::TreePath& path);
    CtTreeIter                      to_ct_tree_iter(Gtk::TreeModel::iterator tree_iter) const;

    void nodes_sequences_fix(Gtk::TreeModel::iterator father_iter, bool process_children);

    const CtTreeModelColumns& get_columns() const { return _columns; }

    void pending_edit_db_bookmarks();
    void pending_rm_db_nodes(const std::vector<gint64>& node_ids);
    const char* get_node_icon(int nodeDepth, const std::string &syntax, guint32 customIconId);

protected:
    Glib::RefPtr<Gdk::Pixbuf> _get_node_icon(int nodeDepth, const std::string &syntax, guint32 customIconId);
    void                      _iter_delete_anchored_widgets(const Gtk::TreeModel::Children& children);

    void _on_textbuffer_modified_changed(Glib::RefPtr<Gtk::TextBuffer> pTextBuffer);
    void _on_textbuffer_insert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text, int bytes);
    void _on_textbuffer_erase(const Gtk::TextBuffer::iterator& range_start, const Gtk::TextBuffer::iterator& range_end);
    void _on_textbuffer_mark_set(const Gtk::TextIter& iter, const Glib::RefPtr<Gtk::TextMark>& rMark);

private:
    CtTreeModelColumns              _columns;
    Glib::RefPtr<Gtk::TreeStore>    _rTreeStore;
    std::list<gint64>               _bookmarks;
    std::set<Glib::ustring>         _usedTags;
    std::map<gint64, Glib::ustring> _nodes_names_dict; // for link tooltips
    std::list<sigc::connection>     _curr_node_sigc_conn;
    CtMainWin*                      _pCtMainWin;
};
