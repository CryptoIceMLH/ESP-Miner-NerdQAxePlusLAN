import { Component } from '@angular/core';
import { TranslateService } from '@ngx-translate/core';
import { Store } from '@ngrx/store';
import * as fromI18n from './@i18n/reducers';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent {
  title = 'axe-os';

  constructor(
    private translate: TranslateService,
    private store: Store<fromI18n.State>
  ) {
    translate.addLangs(['en', 'fr', 'es', 'de']);
    translate.setDefaultLang('en');

    const browserLang = navigator.language.split('-')[0];
    const storedLang = localStorage.getItem('language');

    if (storedLang) {
      translate.use(storedLang);
    } else if (translate.getLangs().includes(browserLang)) {
      translate.use(browserLang);
    } else {
      translate.use('en');
    }

    this.store.select(fromI18n.selectLanguage).subscribe(lang => {
      if (lang) {
        translate.use(lang);
        localStorage.setItem('language', lang);
      }
    });
  }
}
