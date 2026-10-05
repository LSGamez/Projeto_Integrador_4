"""Aplicar com o Unreal principal fechado, ou executar dentro do Cena_Menu aberto."""
import unreal as u,pathlib,json,datetime,shutil,traceback
report={'complete':False}
ROOT=pathlib.Path(u.Paths.project_dir()).resolve()
DEST='/Game/Laboratorio/Materiais/Niveis_Liquido/'
def run():
    level=u.get_editor_subsystem(u.LevelEditorSubsystem)
    world=u.get_editor_subsystem(u.UnrealEditorSubsystem).get_editor_world()
    if not world or world.get_path_name()!='/Game/Cenas/Cena_Menu.Cena_Menu':
        if not level.load_level('/Game/Cenas/Cena_Menu'):raise RuntimeError('Falha abrindo Cena_Menu.')
    actors=list(u.get_editor_subsystem(u.EditorActorSubsystem).get_all_level_actors())
    mappings={
        'Frasco_02':{'Liquido_Ambar':'MI_Laranja_Baixo','Bolhas_Ambar':'MI_Bolhas_Laranja'},
        'Frasco_01':{'Liquido_Verde':'MI_Medio','Bolhas_Verdes':'MI_Bolhas_Medio'},
        'Frasco_05':{'M_Liquido_Violeta':'MI_Azul_Alto','M_Bolhas_Lilas':'MI_Bolhas_Azul'},
    }
    edits=[]
    for name,slots in mappings.items():
        matches=[a for a in actors if isinstance(a,u.GeometryCacheActor) and a.get_actor_label()==name]
        if len(matches)!=1:raise RuntimeError('Frasco ambiguo ou ausente: '+name)
        actor=matches[0];c=actor.get_geometry_cache_component();cache=c.get_editor_property('geometry_cache')
        names=[str(n) for n in cache.get_editor_property('material_slot_names')]
        for slot,newname in slots.items():
            if slot not in names:raise RuntimeError('Material nao encontrado: '+slot)
            material=u.load_asset(DEST+newname)
            if not material:raise RuntimeError('Material preparado ausente: '+newname)
            edits.append((actor,c,names.index(slot),material))
    backup=ROOT/'Saved'/'Backup_Niveis_Liquido'/datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    backup.mkdir(parents=True)
    shutil.copy2(ROOT/'Content/Cenas/Cena_Menu.umap',backup/'Cena_Menu.umap')
    report['backup']=str(backup);report['changes']=[]
    with u.ScopedEditorTransaction('Ajustar altura dos liquidos'):
        for actor,c,slot,material in edits:
            report['changes'].append({'actor':actor.get_actor_label(),'slot':slot,'before':c.get_material(slot).get_path_name(),'after':material.get_path_name()})
            actor.modify();c.modify();c.set_material(slot,material)
        if not level.save_current_level():raise RuntimeError('Falha salvando Cena_Menu.')
    report['complete']=True
try:run()
except:report['error']=traceback.format_exc();u.log_error(report['error'])
(ROOT/'Saved/Niveis_Liquido_Aplicados.json').write_text(json.dumps(report,indent=2),encoding='utf8')
