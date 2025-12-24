import { Component, OnInit, Output, EventEmitter } from '@angular/core';
import { LocalStorageService } from '../../services/local-storage.service';

enum SupportLevel { Safe = 0, Advanced = 1, Pro = 2 }

@Component({
  selector: 'app-advanced-toggle',
  templateUrl: './advanced-toggle.component.html',
  styleUrls: ['./advanced-toggle.component.css']
})
export class AdvancedToggleComponent implements OnInit {
  @Output() advancedToggled = new EventEmitter<SupportLevel>();
  @Output() bdocEnableRequested = new EventEmitter<void>(); // New event for BDOC warnings

  // New: single integer state (persisted)
  public supportLevel: SupportLevel = SupportLevel.Safe;

  // New single-key storage (as requested)
  private storageKey = 'support-level';

  // Debounce for distinguishing single vs double click
  private clickTimer: any = null;
  private readonly dblClickThreshold = 250; // ms

  constructor(private localStorageService: LocalStorageService) {}

  ngOnInit() {
    this.loadState();
    // Emit initial state for parent compatibility
    this.advancedToggled.emit(this.supportLevel);
  }

  // === Public handlers (template) ===========================================
  // Single click -> Safe <-> Advanced  (or Pro -> Advanced)
  toggleAdvanced() {
    if (this.clickTimer) return; // ignore while waiting for potential dblclick
    this.clickTimer = setTimeout(() => {
      if (this.supportLevel === SupportLevel.Pro) {
        this.setSupportLevel(SupportLevel.Safe);
      } else {
        this.setSupportLevel(
          this.supportLevel === SupportLevel.Advanced ? SupportLevel.Safe : SupportLevel.Advanced
        );
      }
      this.clickTimer = null;
    }, this.dblClickThreshold);
  }

  // Double click -> Pro
  onAdvancedDblClick() {
    // Cancel pending single-click action
    if (this.clickTimer) { clearTimeout(this.clickTimer); this.clickTimer = null; }
    this.setSupportLevel(SupportLevel.Pro);
  }

  // === Internal helpers ======================================================
  private setSupportLevel(level: SupportLevel) {
    this.supportLevel = level;

    // Persist as a single integer
    this.saveState();

    // Inform parent (unchanged contract)
    this.advancedToggled.emit(this.supportLevel);
  }

  private saveState() {
    this.localStorageService.setNumber(this.storageKey, this.supportLevel);
  }

  private loadState() {
    // Load stored support level
    const stored = this.localStorageService.getNumber(this.storageKey);
    if (stored === 0 || stored === 1 || stored === 2) {
      this.setSupportLevel(stored as SupportLevel);
      return;
    }
  }
}
